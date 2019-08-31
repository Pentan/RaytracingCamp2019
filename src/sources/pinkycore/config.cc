#include <iostream>
#include <fstream>
#include <cstdio>
#include <nlohmann/json.hpp>

#include "config.h"

using namespace PinkyPi;

bool Config::load(const std::string& path) {
    std::ifstream fs(path);
    if(!fs.is_open()) {
        std::cerr << "config file couldn't open:" << path << std::endl;
        return false;
    }
    
    nlohmann::json jsonRoot;
    fs >> jsonRoot;
    
    if(!jsonRoot.is_object()) {
        std::cerr << "json format error:" << path << std::endl;
        return false;
    }
    
    if(jsonRoot.contains("width")) {
        width = jsonRoot["width"].get<int>();
    }
    if(jsonRoot.contains("height")) {
        height = jsonRoot["height"].get<int>();
    }
    
    return true;
}

