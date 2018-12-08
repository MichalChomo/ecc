#include <iostream>
#include <fstream>
#include "file_loader.h"
#include "constants.h"

int FileLoader::loadFile(int argc, char **argv, std::ifstream &fileStream) {
    if (argc < 2) {
        std::cerr << "No input file specified." << std::endl;
        usage(argv[0]);
        return RET_MISSING_INPUT_FILE;
    }
    fileStream = std::ifstream(argv[1], std::ifstream::binary);
    if (fileStream.fail() || fileStream.eof()) {
        std::cerr << "File is empty or cannot be opened." << std::endl;
        usage(argv[0]);
        return RET_WRONG_INPUT_FILE;
    }
    return RET_OK;
}

void FileLoader::usage(char *programName) {
    std::cerr << "Usage: " << programName << " <input_file>" << std::endl;
}
