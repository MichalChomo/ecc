#include "constants.h"
#include "reed_solomon.h"
#include "ezpwd_rs.h"

int ReedSolomon::encodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) {
    std::vector<char> block;
    std::vector<char> interleavedData;
    size_t inputSize = getStreamSize(inputFileStream);
    size_t blocksCount = inputSize / DATA_BYTES_IN_CODEWORD;
    size_t lastBlockSize = inputSize % DATA_BYTES_IN_CODEWORD;
    size_t parityBytes = BYTES_IN_CODEWORD - DATA_BYTES_IN_CODEWORD;
    size_t interleavedDataSize = blocksCount * BYTES_IN_CODEWORD;
    if (lastBlockSize > 0) {
        interleavedDataSize += lastBlockSize + parityBytes;
    }
    size_t blockIndex = 0;

    try {
        block.resize(DATA_BYTES_IN_CODEWORD);
        interleavedData.resize(interleavedDataSize);
        while (!inputFileStream.eof()) {
            inputFileStream.read(block.data(), DATA_BYTES_IN_CODEWORD);
            if (inputFileStream.eof()) {
                // End of stream, less bytes than DATA_BYTES_IN_CODEWORD might have been read, compute how many
                // and resize the data.
                if (lastBlockSize > 0) {
                    block.resize(lastBlockSize);
                }
            }
            rs.encode(block); // This adds parity bits to the data.
            if (blockIndex == blocksCount) {
                for (size_t i = 0; i < block.size(); ++i) {
                    size_t interleavedIndex = BYTES_IN_CODEWORD * blocksCount + i;
                    interleavedData[interleavedIndex] = block[i];
                }
            } else {
                for (size_t i = 0; i < block.size(); ++i) {
                    size_t interleavedIndex = blockIndex + i * blocksCount;
                    interleavedData[interleavedIndex] = block[i];
                }
            }

            block.resize(DATA_BYTES_IN_CODEWORD);
            ++blockIndex;
        }
        outputFileStream.write(interleavedData.data(), interleavedData.size());
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return RET_ENCODING_FAILED;
    }

    return RET_OK;
}

int ReedSolomon::decodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) {
    std::vector<char> block;
    std::vector<char> interleavedData;
    std::vector<char> deinterleavedData;
    size_t inputSize = getStreamSize(inputFileStream);
    size_t blocksCount = inputSize / BYTES_IN_CODEWORD;
    size_t lastBlockSize = inputSize % BYTES_IN_CODEWORD;

    try {
        interleavedData.resize(inputSize);
        deinterleavedData.resize(inputSize);

        inputFileStream.read(interleavedData.data(), interleavedData.size());
        // Add interleaved blocks.
        for (size_t i = 0; i < blocksCount; ++i) {
            for (size_t j = 0; j < BYTES_IN_CODEWORD; ++j) {
                size_t deinterleavedIndex = i + j * blocksCount;
                deinterleavedData[j + i * BYTES_IN_CODEWORD] = interleavedData[deinterleavedIndex];
            }
        }
        // Add the last block. It is not interleaved.
        deinterleavedData.insert(deinterleavedData.begin() + blocksCount * BYTES_IN_CODEWORD,
                interleavedData.begin() + blocksCount * BYTES_IN_CODEWORD,
                interleavedData.begin() + blocksCount * BYTES_IN_CODEWORD + lastBlockSize);

        // Decode and write interleaved blocks.
        for (size_t i = 0; i < blocksCount; ++i) {
            block.clear();
            block.insert(block.begin(), deinterleavedData.begin() + i * BYTES_IN_CODEWORD,
                         deinterleavedData.begin() + i * BYTES_IN_CODEWORD + BYTES_IN_CODEWORD);
            rs.decode(block);
            block.resize(block.size() - rs.nroots()); // Remove parity bits, decode function doesn't do it.
            outputFileStream.write(block.data(), block.size());
        }

        // Decode and write the last block.
        block.clear();
        block.insert(block.begin(), deinterleavedData.begin() + blocksCount * BYTES_IN_CODEWORD,
                     deinterleavedData.begin() + blocksCount * BYTES_IN_CODEWORD + lastBlockSize);
        rs.decode(block);
        block.resize(block.size() - rs.nroots()); // Remove parity bits, decode function doesn't do it.
        outputFileStream.write(block.data(), block.size());
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return RET_DECODING_FAILED;
    }

    return RET_OK;
}

size_t ReedSolomon::getStreamSize(std::ifstream &inputFileStream) {
    inputFileStream.seekg(0, std::ifstream::end);
    size_t inputSize = inputFileStream.tellg();
    inputFileStream.seekg(0, std::ifstream::beg);
    return inputSize;
}