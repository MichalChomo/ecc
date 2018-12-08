#include "constants.h"
#include "reed_solomon.h"
#include "ezpwd_rs.h"

int ReedSolomon::encodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) {
    std::vector<uint16_t> data;
    char c;
    uint16_t tmp;

    int i = 0;
    for (; !inputFileStream.eof(); ++i) {
        if (i == (DATA_BYTES_IN_CODEWORD - 1)) {
            rs.encode(data);
            for (size_t j = 0; j < data.size(); ++j) {
                c = data.at(j) & 0xff;
                outputFileStream.write(&c, 1);
                c = data.at(j) >> 8;
                outputFileStream.write(&c, 1);
            }
            data.clear();
            i = 0;
        }
        inputFileStream.read(&c, 1);
        tmp = static_cast<uint16_t>(c);
        if (inputFileStream.eof()) {
            rs.encode(data);
            for (size_t j = 0; j < data.size(); ++j) {
                c = data.at(j) & 0xff;
                outputFileStream.write(&c, 1);
                c = data.at(j) >> 8;
                outputFileStream.write(&c, 1);
            }
        }
        inputFileStream.read(&c, 1);
        tmp |= c << 8;
        data.push_back(tmp);
    }


    return RET_OK;
}

int ReedSolomon::decodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) {
    std::vector<uint16_t> data;
    char c;
    uint16_t tmp;

    int i = 0;
    for (; !inputFileStream.eof(); ++i) {
        if (i == (BYTES_IN_CODEWORD - 1)) {
            std::cout << rs.decode(data) << std::endl;
            data.resize(data.size() - rs.nroots());
            for (size_t j = 0; j < data.size(); ++j) {
                c = data.at(j) & 0xff;
                outputFileStream.write(&c, 1);
                c = data.at(j) >> 8;
                outputFileStream.write(&c, 1);
            }
            data.clear();
            i = 0;
        }
        inputFileStream.read(&c, 1);
        tmp = static_cast<uint16_t>(c);
        if (inputFileStream.eof()) {
            std::cout << rs.decode(data) << std::endl;
            data.resize(data.size() - rs.nroots());
            for (size_t j = 0; j < data.size(); ++j) {
                c = data.at(j) & 0xff;
                outputFileStream.write(&c, 1);
                c = data.at(j) >> 8;
                outputFileStream.write(&c, 1);
            }
        }
        inputFileStream.read(&c, 1);
        tmp |= c << 8;
        data.push_back(tmp);
    }

    return RET_OK;
}

int ReedSolomon::getStreamSize(std::ifstream &inputFileStream) {
    inputFileStream.seekg(0, inputFileStream.end);
    int inputSize = inputFileStream.tellg();
    inputFileStream.seekg(0, inputFileStream.beg);
    return inputSize;
}