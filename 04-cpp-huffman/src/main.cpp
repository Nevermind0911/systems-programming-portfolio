#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "HuffmanIO.hpp"
#include "HuffmanTree.hpp"

size_t getHuffmanHeaderSize() {
    return sizeof(int32_t);
}

size_t getAdditionalHeaderSize() {
    return 2 * sizeof(int32_t) + sizeof(uint8_t);
}

size_t getHuffmanFreqTableSize(const Huffman::HuffmanTree &tree) {
    return tree.getFreqTable().size() * (sizeof(char) + sizeof(int));
}

enum Mode { NONE, COMPRESS, DECOMPRESS };

int main(int argc, char *argv[]) {
    Mode mode = NONE;
    std::string inputFile;
    std::string outputFile;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-c") == 0) {
            mode = COMPRESS;
        } else if (strcmp(argv[i], "-u") == 0) {
            mode = DECOMPRESS;
        } else if (strcmp(argv[i], "-f") == 0 ||
                   strcmp(argv[i], "--file") == 0) {
            if (i + 1 < argc) {
                inputFile = argv[++i];
            } else {
                std::cerr << "Error: Missing input file after -f\n";
                return 1;
            }
        } else if (strcmp(argv[i], "-o") == 0 ||
                   strcmp(argv[i], "--output") == 0) {
            if (i + 1 < argc) {
                outputFile = argv[++i];
            } else {
                std::cerr << "Error: Missing output file after -o\n";
                return 1;
            }
        }
    }

    if (mode == NONE) {
        std::cerr
            << "Error: Must specify either compression or decompression mode\n";
        return 1;
    }

    if (inputFile.empty()) {
        std::cerr << "Error: Input file not specified\n";
        return 1;
    }

    if (outputFile.empty()) {
        std::cerr << "Error: Output file not specified\n";
        return 1;
    }

    try {
        if (mode == COMPRESS) {
            std::ifstream inFile(inputFile, std::ios::binary);
            if (!inFile) {
                std::cerr << "Error: Cannot open input file " << inputFile
                          << std::endl;
                return 1;
            }

            inFile.seekg(0, std::ios::end);
            size_t inputSize = inFile.tellg();
            inFile.seekg(0, std::ios::beg);

            std::string inputData(inputSize, '\0');
            inFile.read(&inputData[0], inputSize);
            inFile.close();

            Huffman::HuffmanTree tree(inputData);
            std::vector<int> compressed = tree.compress(inputData);
            uint8_t excessBits = compressed.empty() ? 0 : compressed[0];
            Huffman::CompressedFile compressedFile(
                compressed, tree, tree.getBitCount(), excessBits
            );

            std::ofstream outFile(outputFile, std::ios::binary);
            if (!outFile) {
                std::cerr << "Error: Cannot open output file " << outputFile
                          << std::endl;
                return 1;
            }

            outFile << compressedFile;
            outFile.close();

            std::ifstream checkFile(
                outputFile, std::ios::binary | std::ios::ate
            );
            size_t actualCompressedSize = checkFile.tellg();
            checkFile.close();

            size_t headerSize = getHuffmanHeaderSize();
            size_t freqTableSize = getHuffmanFreqTableSize(tree);
            size_t additionalHeaderSize = getAdditionalHeaderSize();
            size_t tableSize =
                headerSize + freqTableSize + additionalHeaderSize;

            size_t compressedDataSize = actualCompressedSize - tableSize;

            std::cout << inputSize << std::endl;
            std::cout << compressedDataSize << std::endl;
            std::cout << tableSize << std::endl;
        } else {  // DECOMPRESS
            std::ifstream compressedInput(inputFile, std::ios::binary);
            if (!compressedInput) {
                std::cerr << "Error: Cannot open compressed file '" << inputFile
                          << "'\n";
                return 1;
            }

            compressedInput.seekg(0, std::ios::end);
            size_t compressedFileSize = compressedInput.tellg();
            compressedInput.seekg(0, std::ios::beg);

            Huffman::CompressedFile compressedFile;
            compressedInput >> compressedFile;
            compressedInput.close();

            std::vector<int> compressed = compressedFile.getData();
            const Huffman::HuffmanTree &tree = compressedFile.getTree();

            size_t headerSize = getHuffmanHeaderSize();
            size_t freqTableSize = getHuffmanFreqTableSize(tree);
            size_t additionalHeaderSize = getAdditionalHeaderSize();
            size_t tableSize =
                headerSize + freqTableSize + additionalHeaderSize;

            std::string compressedString(compressed.begin(), compressed.end());
            std::vector<int> decompressed = tree.decompress(compressedString);

            std::ofstream outFile(outputFile, std::ios::binary);
            if (!outFile) {
                std::cerr << "Error: Cannot open output file " << outputFile
                          << std::endl;
                return 1;
            }

            for (const auto &byte : decompressed) {
                outFile.put(static_cast<char>(byte));
                if (!outFile) {
                    std::cerr << "Error: Failed to write to output file "
                              << outputFile << std::endl;
                    return 1;
                }
            }
            outFile.close();

            size_t compressedDataSize = compressedFileSize - tableSize;

            std::cout << compressedDataSize << std::endl;
            std::cout << decompressed.size() << std::endl;
            std::cout << tableSize << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
