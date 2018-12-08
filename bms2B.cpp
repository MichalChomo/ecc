#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "file_loader.h"
#include "constants.h"
#include "error_correction_code.h"
#include "reed_solomon.h"

int main(int argc, char *argv[]) {

    std::ifstream inputFileStream;

    int ret = FileLoader::loadFile(argc, argv, inputFileStream);
    if (ret != RET_OK) {
        return ret;
    }

    std::unique_ptr<ErrorCorrectionCode> errorCorrectionCode = std::unique_ptr<ErrorCorrectionCode>(new ReedSolomon());
    std::ofstream outputFileStream(std::string(argv[1]).append(".ok"));
    ret = errorCorrectionCode->decodeFile(inputFileStream, outputFileStream);
    if (ret != RET_OK) {
        return ret;
    }

    return RET_OK;
}