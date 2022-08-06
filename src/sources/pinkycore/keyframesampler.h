#ifndef PINKYPI_KEYFRAMESAMPLER_H
#define PINKYPI_KEYFRAMESAMPLER_H

#include <string>
#include <vector>
#include "pptypes.h"

namespace PinkyPi {
    
    class KeyframeSampler {
    public:
        enum InterpolationType {
            kLinear,
            kStep,
            kCubicSpline
        };
        
        KeyframeSampler()
            : interpolation(kLinear)
        {}
        
        PPFloat* evaluate(PPTimeType time);
        Quaterion evaluateQuaternion(PPTimeType time);
        
        InterpolationType interpolation;
        std::vector<PPTimeType> timeStamps;
        std::vector<PPFloat> sampleBuffer;
        size_t sampleComponents;
    };
}

#endif
