#include <iostream>
#include <fstream>
#include <cstdio>
#include <nlohmann/json.hpp>

#include "config.h"

using namespace PinkyPi;

bool Config::load(const std::string& path) {
    std::fstream fs(path);
    nlohmann::json jsonRoot;
    fs >> jsonRoot;
    
    if(!jsonRoot.is_object()) {
        fprintf(stderr, "json %s format error\n", path.c_str());
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

