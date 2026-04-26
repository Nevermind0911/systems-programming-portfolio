#ifndef HUFFMAN_IO_HPP_INCLUDED
#define HUFFMAN_IO_HPP_INCLUDED

#pragma once

#include <cstdint>
#include <fstream>
#include <vector>
#include "HuffmanTree.hpp"

namespace Huffman {
class HuffmanIO {
public:
    std::vector<int> bitsToBytes(const std::vector<bool> &bits);
    std::vector<bool>
    bytesToBits(const std::vector<int> &bytes, std::size_t count);
};

class CompressedFile {
public:
    CompressedFile() = default;
    explicit CompressedFile(const std::vector<int> &data);
    explicit CompressedFile(
        const std::vector<int> &data,
        HuffmanTree &tree,
        uint32_t bitCount
    );
    explicit CompressedFile(
        const std::vector<int> &data,
        HuffmanTree &tree,
        uint32_t bitCount,
        uint8_t excessBits
    );

    std::vector<int> getData() const;
    HuffmanTree getTree() const;
    uint32_t getBitCount() const;
    uint8_t getExcessBits() const;

    void setData(std::vector<int> &newData);
    void setTree(HuffmanTree &newTree);
    void setBitCount(uint32_t bitCount);
    void setExcessBits(uint8_t excessBits);

    friend std::ofstream &
    operator<<(std::ofstream &ofs, const CompressedFile &file);
    friend std::ifstream &operator>>(std::ifstream &ifs, CompressedFile &file);

private:
    std::vector<int> data;
    HuffmanTree tree;
    uint32_t bitCount;
    uint8_t excessBits;
};
}  // namespace Huffman

#endif  // HUFFMAN_IO_HPP_INCLUDED
