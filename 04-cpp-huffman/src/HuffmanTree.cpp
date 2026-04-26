#include <cstdint>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>
#include "HuffmanIO.hpp"
#include "HuffmanTree.hpp"

namespace Huffman {

HuffmanTreeNode::HuffmanTreeNode(
    std::shared_ptr<HuffmanTreeNode> left,
    std::shared_ptr<HuffmanTreeNode> right
)
    : value('\0'),
      frequency(left->getFrequency() + right->getFrequency()),
      left(left),
      right(right) {
}

HuffmanTreeNode::HuffmanTreeNode(char value, int frequency)
    : value(value), frequency(frequency), left(nullptr), right(nullptr) {
}

HuffmanTreeNode::HuffmanTreeNode(
    char value,
    int frequency,
    std::shared_ptr<HuffmanTreeNode> left,
    std::shared_ptr<HuffmanTreeNode> right
)
    : value(value), frequency(frequency), left(left), right(right) {
}

std::shared_ptr<HuffmanTreeNode> HuffmanTreeNode::getLeft() const {
    return left;
}

std::shared_ptr<HuffmanTreeNode> HuffmanTreeNode::getRight() const {
    return right;
}

char HuffmanTreeNode::getValue() const {
    return value;
}

int HuffmanTreeNode::getFrequency() const {
    return frequency;
}

bool HuffmanTreeNode::isLeaf() const {
    return left == nullptr && right == nullptr;
}

HuffmanTree::HuffmanTree() : bitCount(0), root(nullptr) {
}

HuffmanTree::HuffmanTree(const std::string &data) {
    std::map<char, int> map = calculateFreq(data);
    buildTree(map);
    generateFreqTable();
}

HuffmanTree::HuffmanTree(std::map<char, int> &map) {
    buildTree(map);
    generateFreqTable();
}

std::map<char, int> HuffmanTree::getFreqTable() const {
    return freqTable;
}

uint32_t HuffmanTree::getBitCount() const {
    return bitCount;
}

void HuffmanTree::setBitCount(uint32_t bitCount) {
    this->bitCount = bitCount;
}

std::ostream &operator<<(std::ostream &os, const HuffmanTree &tree) {
    uint32_t tableSize = tree.freqTable.size();
    os.write(reinterpret_cast<const char *>(&tableSize), sizeof(tableSize));

    for (auto it = tree.freqTable.begin(); it != tree.freqTable.end(); it++) {
        char character = it->first;
        int frequency = it->second;
        os.write(&character, sizeof(character));
        os.write(reinterpret_cast<const char *>(&frequency), sizeof(frequency));
    }

    return os;
}

std::istream &operator>>(std::istream &is, HuffmanTree &tree) {
    uint32_t tableSize;
    is.read(reinterpret_cast<char *>(&tableSize), sizeof(tableSize));
    if (!is) {
        throw std::runtime_error("Failed to read tableSize");
    }

    tree.freqTable.clear();
    for (uint32_t i = 0; i < tableSize; i++) {
        char symbol;
        int frequency;
        is.get(symbol);
        is.read(reinterpret_cast<char *>(&frequency), sizeof(frequency));
        if (!is) {
            throw std::runtime_error("Failed to read frequency table entry");
        }
        tree.freqTable[symbol] = frequency;
    }

    tree.buildTree(tree.freqTable);
    return is;
}

void HuffmanTree::buildTree(const std::map<char, int> &map) {
    std::priority_queue<
        std::shared_ptr<HuffmanTreeNode>,
        std::vector<std::shared_ptr<HuffmanTreeNode>>, Compare>
        pq;

    for (const auto &pair : map) {
        pq.push(std::make_shared<HuffmanTreeNode>(pair.first, pair.second));
    }

    if (pq.empty()) {
        root = nullptr;
        return;
    }

    if (pq.size() == 1) {
        std::shared_ptr<HuffmanTreeNode> node = pq.top();
        pq.pop();
        root = std::make_shared<HuffmanTreeNode>(
            '\0', node->getFrequency() + 1, node, nullptr
        );
        return;
    }

    while (pq.size() > 1) {
        std::shared_ptr<HuffmanTreeNode> right = pq.top();
        pq.pop();
        auto left = pq.top();
        pq.pop();
        auto parent = std::make_shared<HuffmanTreeNode>(left, right);
        pq.push(parent);
    }

    if (!pq.empty()) {
        root = pq.top();
    }
}

std::map<char, int> HuffmanTree::calculateFreq(const std::string &data) {
    std::map<char, int> freqTable;

    for (auto &element : data) {
        freqTable[element]++;
    }
    return freqTable;
}

std::map<char, int> HuffmanTree::generateFreqTable() {
    freqTable.clear();
    codeTable.clear();

    if (!root) {
        return freqTable;
    }

    std::vector<bool> currentCode;
    traverseTree(root, currentCode);

    return freqTable;
}

void HuffmanTree::traverseTree(
    std::shared_ptr<HuffmanTreeNode> node,
    std::vector<bool> &code
) {
    if (!node) {
        return;
    }

    if (node->isLeaf()) {
        char symbol = node->getValue();
        codeTable[symbol] = code;
        freqTable[symbol] = node->getFrequency();
        return;
    }

    if (node->getLeft()) {
        code.push_back(false);
        traverseTree(node->getLeft(), code);
        code.pop_back();
    }

    if (node->getRight()) {
        code.push_back(true);
        traverseTree(node->getRight(), code);
        code.pop_back();
    }
}

std::vector<int> HuffmanTree::compress(std::string &data) {
    std::vector<bool> bits;

    if (data.empty() || codeTable.empty()) {
        bitCount = 0;
        return std::vector<int>();
    }

    for (const auto &byte : data) {
        if (codeTable.count(byte)) {
            const std::vector<bool> &code = codeTable[byte];
            bits.insert(bits.end(), code.begin(), code.end());
        }
    }

    bitCount = bits.size();
    HuffmanIO converter;
    std::vector<int> bytes = converter.bitsToBytes(bits);

    int excessBits = (8 - (bitCount % 8)) % 8;
    std::vector<int> result;
    result.push_back(excessBits);
    result.insert(result.end(), bytes.begin(), bytes.end());

    return result;
}

std::vector<int> HuffmanTree::decompress(std::string &data) const {
    std::vector<int> bytes;
    for (unsigned char c : data) {
        bytes.push_back(static_cast<int>(c));
    }

    if (bytes.empty()) {
        return std::vector<int>();
    }

    std::vector<int> dataBytes(bytes.begin() + 1, bytes.end());

    HuffmanIO converter;
    std::vector<bool> bits = converter.bytesToBits(dataBytes, bitCount);

    std::vector<int> result;

    if (!root || bitCount == 0) {
        return result;
    }

    if (root->isLeaf()) {
        size_t repeatCount = bitCount;
        for (size_t i = 0; i < repeatCount; i++) {
            result.push_back(static_cast<unsigned char>(root->getValue()));
        }
        return result;
    }

    std::shared_ptr<HuffmanTreeNode> currentNode = root;
    for (size_t i = 0; i < bits.size(); i++) {
        bool bit = bits[i];
        if (bit) {
            currentNode = currentNode->getRight();
        } else {
            currentNode = currentNode->getLeft();
        }

        if (!currentNode) {
            currentNode = root;
            continue;
        }

        if (currentNode->isLeaf()) {
            result.push_back(static_cast<unsigned char>(currentNode->getValue())
            );
            currentNode = root;
        }
    }

    return result;
}

}  // namespace Huffman
