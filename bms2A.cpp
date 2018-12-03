#include <iostream>
#include <fstream>
#include <vector>
#include "file_loader.h"
#include "constants.h"

int main(int argc, char *argv[]) {

    std::ifstream inputFileStream;

    int ret = FileLoader::loadFile(argc, argv, inputFileStream);
    if (ret != RET_OK) {
        return ret;
    }

    std::cout << inputFileStream.get() << std::endl;
    std::cout << inputFileStream.get() << std::endl;
    std::cout << inputFileStream.get() << std::endl;
    std::cout << inputFileStream.get() << std::endl;

    return RET_OK;
}