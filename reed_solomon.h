#ifndef ECC_REEDSOLOMON_H
#define ECC_REEDSOLOMON_H

#include <vector>
#include "error_correction_code.h"
#include "ezpwd_rs.h"

class ReedSolomon : public ErrorCorrectionCode {

    /**
     * Parity bytes = BYTES_IN_CODEWORD - DATA_BYTES_IN_CODEWORD
     *
     * Symbol size = log₂(BYTES_IN_CODEWORD - 1)
     */
    static const int BYTES_IN_CODEWORD = 255;
    static const int DATA_BYTES_IN_CODEWORD = 149;

public:
    /**
     * Encode the input stream with Reed-Solomon code = add parity bytes and output the result to the output stream.
     * @param inputFileStream
     * @param outputFileStream
     * @return RET_OK on success, RET_ENCODING_FAILED on failure.
     */
    int encodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) override;

    /**
     * Decode the input stream with Reed-Solomon code = remove parity bytes and output the result to the output stream.
     * @param inputFileStream
     * @param outputFileStream
     * @return
     */
    int decodeFile(std::ifstream &inputFileStream, std::ofstream &outputFileStream) override;

private:

    /**
     * Reed-Solomon implementation taken from here: https://github.com/pjkundert/ezpwd-reed-solomon
     */
    ezpwd::RS<BYTES_IN_CODEWORD, DATA_BYTES_IN_CODEWORD> rs;

    /**
     * @param inputFileStream
     * @return Stream size in bytes.
     */
    int getStreamSize(std::ifstream &inputFileStream);

    void interleave(std::vector<char> &data, std::vector<char> &interleavedData);

    void deinterleave(std::vector<char> &interleavedData, std::vector<char> &data);

};


#endif //ECC_REEDSOLOMON_H