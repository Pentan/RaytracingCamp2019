#include <iostream>
#include <fstream>
#include <cstdio>
#include <nlohmann/json.hpp>

#include "config.h"

using namespace PinkyPi;

namespace {
    template <typename T> T GetConfigValue(nlohmann::json &json, std::string key, T defaultValue) {
        if(json.contains(key)) {
            return json[key].get<T>();
        }
        return defaultValue;
    }
}

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
    
    width = GetConfigValue<int>(jsonRoot, "width", width);
    height = GetConfigValue<int>(jsonRoot, "height", height);
    
    frames = GetConfigValue<int>(jsonRoot, "frames", frames);
    startFrame = GetConfigValue<int>(jsonRoot, "startFrame", startFrame);
    framesPerSecond = GetConfigValue<double>(jsonRoot, "framesPerSecond", framesPerSecond);
    
    exposureSecond = GetConfigValue<double>(jsonRoot, "exposureSecond", exposureSecond);
    exposureSlice = GetConfigValue<int>(jsonRoot, "exposureSlice", exposureSlice);
    
    samplesPerPixel = GetConfigValue<int>(jsonRoot, "samplesPerPixel", samplesPerPixel);
    pixelSubSamples = GetConfigValue<int>(jsonRoot, "pixelSubSamples", pixelSubSamples);
    
    minDepth = GetConfigValue<int>(jsonRoot, "minDepth", minDepth);
    maxDepth = GetConfigValue<int>(jsonRoot, "maxDepth", maxDepth);
    minRussianRouletteCutOff = GetConfigValue<float>(jsonRoot, "minRussianRouletteCutOff", minRussianRouletteCutOff);
    
    framebufferStockCount = GetConfigValue<int>(jsonRoot, "framebufferStockCount", framebufferStockCount);
    tileSize = GetConfigValue<int>(jsonRoot, "tileSize", tileSize);
    scrambleTile = GetConfigValue<bool>(jsonRoot, "scrambleTile", scrambleTile);
    
    limitSec = GetConfigValue<double>(jsonRoot, "limitSec", limitSec);
    progressIntervalSec = GetConfigValue<double>(jsonRoot, "progressIntervalSec", progressIntervalSec);
    maxThreads = GetConfigValue<int>(jsonRoot, "maxThreads", maxThreads);
    
    quietProgress = GetConfigValue<bool>(jsonRoot, "quietProgress", quietProgress);
    waitUntilFinish = GetConfigValue<bool>(jsonRoot, "waitUntilFinish", waitUntilFinish);
    
    inputFile = GetConfigValue<std::string>(jsonRoot, "inputFile", inputFile);
    outputDir = GetConfigValue<std::string>(jsonRoot, "outputDir", outputDir);
    outputName = GetConfigValue<std::string>(jsonRoot, "outputName", outputName);
    outputExt = GetConfigValue<std::string>(jsonRoot, "outputExt", outputExt);
    
    return true;
}

void Config::parseOptions(int argc, char* argv[]) {
    for(int i = 1; i < argc; i++) {
        char* v = argv[i];
        bool hasnext = (i + 1) < argc;
        
        if(strcmp(v, "-i") == 0 && hasnext) {
            inputFile = argv[i + 1];
            i += 1;
        } else if(strcmp(v, "-on") == 0 && hasnext) {
            outputName = argv[i + 1];
            i += 1;
        } else if(strcmp(v, "-od") == 0 && hasnext) {
            outputDir = argv[i + 1];
            i += 1;
        } else if(strcmp(v, "-oe") == 0 && hasnext) {
            outputExt = argv[i + 1];
            i += 1;
        } else if(strcmp(v, "-j") == 0 && hasnext) {
            maxThreads = std::atoi(argv[i + 1]);
            i += 1;
        } else if(strcmp(v, "-w") == 0 && hasnext) {
            width = std::atoi(argv[i + 1]);
            i += 1;
        } else if(strcmp(v, "-h") == 0 && hasnext) {
            height = std::atoi(argv[i + 1]);
            i += 1;
        } else if(strcmp(v, "-f") == 0 && hasnext) {
            frames = std::atoi(argv[i + 1]);
            i += 1;
        } else if(strcmp(v, "-sf") == 0 && hasnext) {
            startFrame = std::atoi(argv[i + 1]);
            i += 1;
        } else if(strcmp(v, "-fps") == 0 && hasnext) {
            framesPerSecond = std::atof(argv[i + 1]);
            i += 1;
        } else if(strcmp(v, "-s") == 0 && hasnext) {
            samplesPerPixel = std::atoi(argv[i + 1]);
            i += 1;
        } else if(strcmp(v, "-ss") == 0 && hasnext) {
            pixelSubSamples = std::atoi(argv[i + 1]);
            i += 1;
        }
    }
}


