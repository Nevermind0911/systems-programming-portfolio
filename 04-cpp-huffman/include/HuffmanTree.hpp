#ifndef HUFFMAN_TREE_HPP_INCLUDED
#define HUFFMAN_TREE_HPP_INCLUDED

#include <cstdint>
#include <istream>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace Huffman {
class HuffmanTreeNode {
public:
    HuffmanTreeNode(
        std::shared_ptr<HuffmanTreeNode> left,
        std::shared_ptr<HuffmanTreeNode> right
    );
    HuffmanTreeNode(char value, int frequency);
    HuffmanTreeNode(
        char value,
        int frequency,
        std::shared_ptr<HuffmanTreeNode> left,
        std::shared_ptr<HuffmanTreeNode> right
    );

    std::shared_ptr<HuffmanTreeNode> getLeft() const;
    std::shared_ptr<HuffmanTreeNode> getRight() const;
    char getValue() const;
    int getFrequency() const;
    bool isLeaf() const;

private:
    char value;
    int frequency;
    std::shared_ptr<HuffmanTreeNode> left;
    std::shared_ptr<HuffmanTreeNode> right;
};

class HuffmanTree {
public:
    HuffmanTree();
    HuffmanTree(const std::string &data);
    HuffmanTree(std::map<char, int> &map);

    std::vector<int> compress(std::string &data);
    std::vector<int> decompress(std::string &data) const;

    std::map<char, int> getFreqTable() const;
    uint32_t getBitCount() const;

    void setBitCount(uint32_t bitCount);

    friend std::ostream &operator<<(std::ostream &os, const HuffmanTree &tree);
    friend std::istream &operator>>(std::istream &is, HuffmanTree &tree);

private:
    uint32_t bitCount;
    std::shared_ptr<HuffmanTreeNode> root;
    std::map<char, int> freqTable;
    std::map<char, std::vector<bool>> codeTable;

    void buildTree(const std::map<char, int> &map);
    std::map<char, int> generateFreqTable();
    void traverseTree(
        std::shared_ptr<HuffmanTreeNode> node,
        std::vector<bool> &code
    );
    std::map<char, int> calculateFreq(const std::string &data);
};

struct Compare {
public:
    bool operator()(
        const std::shared_ptr<HuffmanTreeNode> first,
        const std::shared_ptr<HuffmanTreeNode> second
    ) const {
        return first->getFrequency() > second->getFrequency();
    }
};
}  // namespace Huffman

#endif  // HUFFMAN_TREE_HPP_INCLUDED
