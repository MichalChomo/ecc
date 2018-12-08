#ifndef ECC_ERROR_CORRECTION_CODE_H
#define ECC_ERROR_CORRECTION_CODE_H

#include <fstream>

class ErrorCorrectionCode {

public:
    /**
     * Encode the input stream with an error correction code and output the result to the output stream.
     * @param inputFileStream
     * @param outputFileStream
     * @return RET_OK on success, RET_ENCODING_FAILED on failure.
     */
    virtual int encodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) = 0;

    /**
     * Decode the input stream, possibly correct errors, and output the result to the output stream.
     * @param inputFileStream
     * @param outputFileStream
     * @return RET_OK on success, RET_DECODING_FAILED on failure.
     */
    virtual int decodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) = 0;

};

#endif //ECC_ERROR_CORRECTION_CODE_H