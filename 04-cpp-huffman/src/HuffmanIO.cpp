#include <cstddef>
#include <cstdint>
#include <vector>
#include "HuffmanIO.hpp"

namespace Huffman {

std::vector<int> HuffmanIO::bitsToBytes(const std::vector<bool> &bits) {
    int bytesCount = (bits.size() + 7) / 8;
    std::vector<int> bytes(bytesCount, 0);

    for (size_t i = 0; i < bits.size(); i++) {
        if (bits[i]) {
            int byteIndex = i / 8;
            int bitPosition = 7 - (i % 8);
            bytes[byteIndex] = bytes[byteIndex] | (1 << bitPosition);
        }
    }
    return bytes;
}

std::vector<bool>
HuffmanIO::bytesToBits(const std::vector<int> &bytes, std::size_t count) {
    std::vector<bool> bits(count);
    for (size_t i = 0; i < count && i / 8 < bytes.size(); ++i) {
        int byteIndex = i / 8;
        int bitPosition = 7 - (i % 8);
        bits[i] = (bytes[byteIndex] >> bitPosition) & 1;
    }
    return bits;
}

CompressedFile::CompressedFile(const std::vector<int> &data) : data(data) {
}

CompressedFile::CompressedFile(
    const std::vector<int> &data,
    HuffmanTree &tree,
    uint32_t bitCount
)
    : data(data), tree(tree), bitCount(bitCount) {
}

CompressedFile::CompressedFile(
    const std::vector<int> &data,
    HuffmanTree &tree,
    uint32_t bitCount,
    uint8_t excessBits
)
    : data(data), tree(tree), bitCount(bitCount), excessBits(excessBits) {
}

std::vector<int> CompressedFile::getData() const {
    return data;
}

HuffmanTree CompressedFile::getTree() const {
    return tree;
}

uint32_t CompressedFile::getBitCount() const {
    return bitCount;
}

uint8_t CompressedFile::getExcessBits() const {
    return excessBits;
}

void CompressedFile::setData(std::vector<int> &newData) {
    data = newData;
}

void CompressedFile::setTree(HuffmanTree &newTree) {
    tree = newTree;
}

void CompressedFile::setBitCount(uint32_t bitCount) {
    this->bitCount = bitCount;
}

void CompressedFile::setExcessBits(uint8_t excessBits) {
    this->excessBits = excessBits;
}

std::ofstream &operator<<(std::ofstream &ofs, const CompressedFile &file) {
    ofs << file.tree;

    uint32_t dataSize = file.data.size();
    ofs.write(reinterpret_cast<const char *>(&dataSize), sizeof(dataSize));

    uint32_t bitCount = file.bitCount;
    ofs.write(reinterpret_cast<const char *>(&bitCount), sizeof(bitCount));

    uint8_t excessBits = file.excessBits;
    ofs.write(reinterpret_cast<const char *>(&excessBits), sizeof(excessBits));

    for (size_t i = 0; i < file.data.size(); i++) {
        ofs.put((char)file.data[i]);
    }

    return ofs;
}

std::ifstream &operator>>(std::ifstream &ifs, CompressedFile &file) {
    ifs >> file.tree;

    uint32_t dataSize;
    ifs.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
    if (!ifs) {
        throw std::runtime_error("Failed to read dataSize");
    }

    uint32_t bitCount;
    ifs.read(reinterpret_cast<char *>(&bitCount), sizeof(bitCount));
    if (!ifs) {
        throw std::runtime_error("Failed to read bitCount");
    }
    file.setBitCount(bitCount);

    uint8_t excessBits;
    ifs.read(reinterpret_cast<char *>(&excessBits), sizeof(excessBits));
    if (!ifs) {
        throw std::runtime_error("Failed to read excessBits");
    }
    file.setExcessBits(excessBits);

    file.data.resize(dataSize);
    for (uint32_t i = 0; i < dataSize; i++) {
        char byte;
        ifs.get(byte);
        if (!ifs) {
            throw std::runtime_error("Failed to read data byte");
        }
        file.data[i] = static_cast<unsigned char>(byte);
    }

    file.tree.setBitCount(bitCount);

    return ifs;
}

}  // namespace Huffman
