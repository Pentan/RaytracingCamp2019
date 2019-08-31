#ifndef PINKYPI_CONFIG_H
#define PINKYPI_CONFIG_H

#include <string>
#include <pinkycore/pptypes.h>

namespace PinkyPi {
    class Config {
    public:
        int width;
        int height;

        int samplesPerPixel;
        int pixelSubSamples;

        int minDepth;
        int maxDepth;
        float minRussianRouletteCutOff;
        
        int tileSize;
        bool scrambleTile;
        
        double limitSec;
        double progressIntervalSec;
        
        bool quietProgress;
        bool waitUntilFinish;
        
        std::string inputFile;
        std::string outputFile;

    public:
        Config():
            width(320),
            height(270),
            samplesPerPixel(4),
            pixelSubSamples(2),
            minDepth(1),
            maxDepth(4),
            minRussianRouletteCutOff(0.005),
            tileSize(64),
            scrambleTile(true),
            limitSec(60.0),
            progressIntervalSec(-1.0),
            quietProgress(false),
            waitUntilFinish(true),
            inputFile(""),
            outputFile("output.png")
        {
        }
        
        bool load(const std::string& path);
    };
}

#endif
