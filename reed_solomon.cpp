#include "constants.h"
#include "reed_solomon.h"
#include "ezpwd_rs.h"

int ReedSolomon::encodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) {
    std::vector<char> data;
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
            rs.encode(data); // This adds parity bits to the data.
            // Size should be BYTES_IN_CODEWORD except when less bytes than DATA_BYTES_IN_CODEWORD were read.
            outputFileStream.write(data.data(), data.size());
            data.resize(DATA_BYTES_IN_CODEWORD);
        }
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return RET_ENCODING_FAILED;
    }

    return RET_OK;
}

int ReedSolomon::decodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) {
    std::vector<char> data;
    int inputSize = getStreamSize(inputFileStream);

    try {
        data.resize(BYTES_IN_CODEWORD);
        while (!inputFileStream.eof()) {
            inputFileStream.read(data.data(), BYTES_IN_CODEWORD);
            if (inputFileStream.eof()) {
                // End of stream, less bytes than BYTES_IN_CODEWORD might have been read, compute how many and resize
                // the data.
                data.resize(inputSize % BYTES_IN_CODEWORD);
            }
            rs.decode(data);
            data.resize(data.size() - rs.nroots()); // Remove parity bits, decode function doesn't do it.
            outputFileStream.write(data.data(), data.size());
            data.resize(BYTES_IN_CODEWORD);
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