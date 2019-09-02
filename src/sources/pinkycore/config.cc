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
    if(jsonRoot.contains("samplesPerPixel")) {
        samplesPerPixel = jsonRoot["samplesPerPixel"].get<int>();
    }
    if(jsonRoot.contains("pixelSubSamples")) {
        pixelSubSamples = jsonRoot["pixelSubSamples"].get<int>();
    }
    if(jsonRoot.contains("minDepth")) {
        minDepth = jsonRoot["minDepth"].get<int>();
    }
    if(jsonRoot.contains("maxDepth")) {
        maxDepth = jsonRoot["maxDepth"].get<int>();
    }
    if(jsonRoot.contains("minRussianRouletteCutOff")) {
        minRussianRouletteCutOff = jsonRoot["minRussianRouletteCutOff"].get<double>();
    }
    if(jsonRoot.contains("tileSize")) {
        tileSize = jsonRoot["tileSize"].get<int>();
    }
    if(jsonRoot.contains("scrambleTile")) {
        scrambleTile = jsonRoot["scrambleTile"].get<bool>();
    }
    if(jsonRoot.contains("limitSec")) {
        limitSec = jsonRoot["limitSec"].get<double>();
    }
    if(jsonRoot.contains("progressIntervalSec")) {
        progressIntervalSec = jsonRoot["progressIntervalSec"].get<double>();
    }
    if(jsonRoot.contains("quietProgress")) {
        quietProgress = jsonRoot["quietProgress"].get<bool>();
    }
    if(jsonRoot.contains("waitUntilFinish")) {
        waitUntilFinish = jsonRoot["waitUntilFinish"].get<bool>();
    }
    if(jsonRoot.contains("inputFile")) {
        inputFile = jsonRoot["inputFile"].get<std::string>();
    }
    if(jsonRoot.contains("outputFile")) {
        outputFile = jsonRoot["outputFile"].get<std::string>();
    }
    
    return true;
}

