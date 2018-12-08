#ifndef ECC_ERROR_CORRECTION_CODE_H
#define ECC_ERROR_CORRECTION_CODE_H

#include <fstream>

class ErrorCorrectionCode {

public:
    virtual int encodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) = 0;

    virtual int decodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) = 0;

};

#endif //ECC_ERROR_CORRECTION_CODE_H