#include "constants.h"
#include "reed_solomon.h"
#include "ezpwd_rs.h"

int ReedSolomon::encodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) {
    std::vector<char> data;
    int inputSize = getStreamSize(inputFileStream);

    data.resize(DATA_BYTES_IN_CODEWORD);
    while (!inputFileStream.eof()) {
        inputFileStream.read(data.data(), DATA_BYTES_IN_CODEWORD);
        if (inputFileStream.eof()) {
            data.resize(inputSize % DATA_BYTES_IN_CODEWORD);
        }
        rs.encode(data);
        outputFileStream.write(data.data(), data.size());
        data.resize(DATA_BYTES_IN_CODEWORD);
    }

    return RET_OK;
}

int ReedSolomon::decodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) {
    std::vector<char> data;
    int inputSize = getStreamSize(inputFileStream);

    data.resize(BYTES_IN_CODEWORD);
    while (!inputFileStream.eof()) {
        inputFileStream.read(data.data(), BYTES_IN_CODEWORD);
        if (inputFileStream.eof()) {
            data.resize(inputSize % BYTES_IN_CODEWORD);
        }
        rs.decode(data);
        data.resize(data.size() - rs.nroots());
        outputFileStream.write(data.data(), data.size());
        data.resize(BYTES_IN_CODEWORD);
    }

    return RET_OK;
}

int ReedSolomon::getStreamSize(std::ifstream &inputFileStream) {
    inputFileStream.seekg(0, inputFileStream.end);
    int inputSize = inputFileStream.tellg();
    inputFileStream.seekg(0, inputFileStream.beg);
    return inputSize;
}