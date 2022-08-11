#include <iostream>
#include <sstream>

#include <doctest.h>
#include "../testsupport.h"

#include <pinkycore/pptypes.h>
#include <pinkycore/config.h>

using namespace PinkyPi;

TEST_CASE("Load test [Config]") {
    std::string path(PINKYPI_TEST_DATA_DIR"/config/testconfig00.json");
    
    Config config;
    bool loaded = config.load(path);
    REQUIRE(loaded);
    
    REQUIRE_EQ(config.width, 1280);
    REQUIRE_EQ(config.height, 720);
    
    REQUIRE_EQ(config.frames, 30);
    REQUIRE(config.framesPerSecond == doctest::Approx(29.9).epsilon(0.01));
    
    REQUIRE(config.exposureSecond == doctest::Approx(0.008).epsilon(0.0001));
    REQUIRE_EQ(config.exposureSlice, 4);
    
    REQUIRE_EQ(config.samplesPerPixel, 16);
    REQUIRE_EQ(config.pixelSubSamples, 4);
    
    REQUIRE_EQ(config.minDepth, 2);
    REQUIRE_EQ(config.maxDepth, 8);
    REQUIRE(config.minRussianRouletteCutOff == doctest::Approx(0.05).epsilon(0.00001));
    
    REQUIRE_EQ(config.framebufferStockCount, 5);
    REQUIRE_EQ(config.tileSize, 256);
    REQUIRE_EQ(config.scrambleTile, false);
    
    REQUIRE(config.limitSec == doctest::Approx(300.0).epsilon(0.01));
    REQUIRE(config.progressIntervalSec == doctest::Approx(2.0).epsilon(0.01));
    REQUIRE_EQ(config.maxThreads, 32);
    
    REQUIRE_EQ(config.quietProgress, true);
    REQUIRE_EQ(config.waitUntilFinish, false);
    
    REQUIRE_EQ(config.inputFile, std::string("testinput.gltf"));
    REQUIRE_EQ(config.outputDir, std::string("output"));
    REQUIRE_EQ(config.outputName, std::string("testoutput"));
    REQUIRE_EQ(config.outputExt, std::string("jpg"));
}

