//
// Created by michal on 3.12.2018.
//

#ifndef ECC_FILELOADER_H
#define ECC_FILELOADER_H

#include <string>

class FileLoader {

public:
    static int loadFile(int argc, char **argv, std::ifstream &fileStream);

private:
    static void usage(char *programName);
};

#endif //ECC_FILELOADER_H
