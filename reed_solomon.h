#ifndef ECC_REEDSOLOMON_H
#define ECC_REEDSOLOMON_H

#include <vector>
#include "error_correction_code.h"
#include "ezpwd_rs.h"

class ReedSolomon : public ErrorCorrectionCode {

    static const int BYTES_IN_CODEWORD = 255;
    static const int DATA_BYTES_IN_CODEWORD = 149;

public:
    int encodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) override;

    int decodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) override;

private:

    ezpwd::RS<BYTES_IN_CODEWORD, DATA_BYTES_IN_CODEWORD> rs;

    int getStreamSize(std::ifstream &inputFileStream);

};


#endif //ECC_REEDSOLOMON_H