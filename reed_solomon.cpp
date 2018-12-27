#include "constants.h"
#include "reed_solomon.h"
#include "ezpwd_rs.h"

int ReedSolomon::encodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) {
    std::vector<char> data;
    std::vector<char> interleavedData;
    std::vector<std::vector<char>> blocks;
    int inputSize = getStreamSize(inputFileStream);

    try {
        data.resize(DATA_BYTES_IN_CODEWORD);
        while (!inputFileStream.eof()) {
            inputFileStream.read(data.data(), DATA_BYTES_IN_CODEWORD);
            if (inputFileStream.eof()) {
                // End of stream, less bytes than DATA_BYTES_IN_CODEWORD might have been read, compute how many
                // and resize the data.
                data.resize(inputSize % DATA_BYTES_IN_CODEWORD);
            }
            std::cout << data[0] << data[1] << std::endl;
            interleave(data, interleavedData);
            rs.encode(interleavedData); // This adds parity bits to the data.
            blocks.push_back(interleavedData);
        }
        std::cout << blocks.size() << std::endl;
        for (unsigned int i = 0; i < BLOCK_INTERLEAVING_LENGTH; ++i) {
            for (unsigned int j = i; j < blocks.size() - 1; j+= BLOCK_INTERLEAVING_LENGTH) {
                outputFileStream.write(blocks[j].data(), blocks[j].size());
            }
        }
        outputFileStream.write(blocks.back().data(), blocks.back().size());
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return RET_ENCODING_FAILED;
    }

    return RET_OK;
}

int ReedSolomon::decodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) {
    std::vector<char> data;
    std::vector<char> interleavedData;
    std::vector<std::vector<char>> blocks;
    int inputSize = getStreamSize(inputFileStream);

    try {
        interleavedData.resize(BYTES_IN_CODEWORD);
        while (!inputFileStream.eof()) {
            inputFileStream.read(interleavedData.data(), BYTES_IN_CODEWORD);
            if (inputFileStream.eof()) {
                // End of stream, less bytes than BYTES_IN_CODEWORD might have been read, compute how many and resize
                // the data.
                interleavedData.resize(inputSize % BYTES_IN_CODEWORD);
            }
            rs.decode(interleavedData);
            interleavedData.resize(interleavedData.size() - rs.nroots()); // Remove parity bits, decode function doesn't do it.
            deinterleave(interleavedData, data);
            blocks.push_back(data);
            interleavedData.resize(BYTES_IN_CODEWORD);
        }
        std::cout << blocks.size() << std::endl;
//        for (auto block : blocks) {
//            std::cout << block[0] << block[1] << std::endl;
//        }
        unsigned int kk = (blocks.size() % BLOCK_INTERLEAVING_LENGTH);
        unsigned int jj = blocks.size() / BLOCK_INTERLEAVING_LENGTH;
        std::cout << kk << std::endl;
        std::cout << jj << std::endl;
        for (unsigned int i = 0; i < BLOCK_INTERLEAVING_LENGTH; ++i) {
            for (unsigned int j = i, k = 0; j < blocks.size() - 1; ++k) {
                if (i == BLOCK_INTERLEAVING_LENGTH - 1 && k >= kk) {
                    break;
                }
                std::cout << blocks[j][0] << blocks[j][1] << " k: " << k << " j: " << j << " i: " << i << std::endl;
                outputFileStream.write(blocks[j].data(), blocks[j].size());
                if (k >= kk) {
                    j+= jj;
                } else {
                    j+= jj + 1;
                }
            }
        }
        outputFileStream.write(blocks.back().data(), blocks.back().size());
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return RET_DECODING_FAILED;
    }

    return RET_OK;
}

int ReedSolomon::getStreamSize(std::ifstream &inputFileStream) {
    inputFileStream.seekg(0, std::ifstream::end);
    int inputSize = inputFileStream.tellg();
    inputFileStream.seekg(0, std::ifstream::beg);
    return inputSize;
}

void ReedSolomon::interleave(std::vector<char> &data, std::vector<char> &interleavedData) {
    int blockLength = data.size() / SYMBOL_INTERLEAVING_LENGTH;

    interleavedData.clear();
    interleavedData.resize(data.size());
    for (int i = 0; i < SYMBOL_INTERLEAVING_LENGTH; ++i) {
        for (int j = 0; j < blockLength; ++j) {
            interleavedData[i * blockLength + j] = data[j * SYMBOL_INTERLEAVING_LENGTH + i];
        }
    }
    for (int i = blockLength * SYMBOL_INTERLEAVING_LENGTH; i < static_cast<int>(data.size()); ++i) {
        interleavedData[i] = data[i];
    }
}

void ReedSolomon::deinterleave(std::vector<char> &interleavedData, std::vector<char> &data) {
    int blockLength = interleavedData.size() / SYMBOL_INTERLEAVING_LENGTH;

    data.clear();
    data.resize(interleavedData.size());
    for (int i = 0; i < SYMBOL_INTERLEAVING_LENGTH; ++i) {
        for (int j = 0; j < blockLength; ++j) {
            data[j * SYMBOL_INTERLEAVING_LENGTH + i] = interleavedData[i * blockLength + j];
        }
    }
    for (int i = blockLength * SYMBOL_INTERLEAVING_LENGTH; i < static_cast<int>(data.size()); ++i) {
        data[i] = interleavedData[i];
    }
}