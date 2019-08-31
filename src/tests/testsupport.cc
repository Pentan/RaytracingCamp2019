#include <sstream>

#include <cstdio>
#include <dirent.h>
#include <sys/stat.h>

#include "testsupport.h"

using namespace PinkyPi;

void PinkyPi::CheckTestOutputDir(std::string dirpath) {
    DIR *dir = opendir(PINKYPI_TEST_OUTPUT_DIR);
    if(dir == NULL) {
        mkdir(PINKYPI_TEST_OUTPUT_DIR, ACCESSPERMS);
    } else {
        closedir(dir);
    }
    
    std::stringstream ss;
    ss << PINKYPI_TEST_OUTPUT_DIR << "/" << dirpath;
    std::string fullpath = ss.str();
    
    dir = opendir(fullpath.c_str());
    if(dir == NULL) {
        mkdir(fullpath.c_str(), ACCESSPERMS);
    } else {
        closedir(dir);
    }
}
