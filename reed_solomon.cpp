#include "constants.h"
#include "reed_solomon.h"
#include "ezpwd_rs.h"

int ReedSolomon::encodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) {
    std::vector<char> data;
    std::vector<char> interleavedData;
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
            interleave(data, interleavedData);
            rs.encode(interleavedData); // This adds parity bits to the data.
            // Size should be BYTES_IN_CODEWORD except when less bytes than DATA_BYTES_IN_CODEWORD were read.
            outputFileStream.write(interleavedData.data(), interleavedData.size());
        }
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return RET_ENCODING_FAILED;
    }

    return RET_OK;
}

int ReedSolomon::decodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) {
    std::vector<char> data;
    std::vector<char> interleavedData;
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
            outputFileStream.write(data.data(), data.size());
            interleavedData.resize(BYTES_IN_CODEWORD);
        }
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
    int interleavingLength = 64;
    int blockLength = data.size() / interleavingLength;

    interleavedData.clear();
    interleavedData.resize(data.size());
    for (int i = 0; i < interleavingLength; ++i) {
        for (int j = 0; j < blockLength; ++j) {
            interleavedData[i * blockLength + j] = data[j * interleavingLength + i];
        }
    }
    for (int i = blockLength * interleavingLength; i < static_cast<int>(data.size()); ++i) {
        interleavedData[i] = data[i];
    }
}

void ReedSolomon::deinterleave(std::vector<char> &interleavedData, std::vector<char> &data) {
    int interleavingLength = 64;
    int blockLength = interleavedData.size() / interleavingLength;

    data.clear();
    data.resize(interleavedData.size());
    for (int i = 0; i < interleavingLength; ++i) {
        for (int j = 0; j < blockLength; ++j) {
            data[j * interleavingLength + i] = interleavedData[i * blockLength + j];
        }
    }
    for (int i = blockLength * interleavingLength; i < static_cast<int>(data.size()); ++i) {
        data[i] = interleavedData[i];
    }
}