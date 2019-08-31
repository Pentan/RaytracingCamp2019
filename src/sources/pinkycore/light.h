#ifndef PINKYPI_LIGHT_H
#define PINKYPI_LIGHT_H

#include <string>
#include <vector>
#include <memory>
#include "pptypes.h"

namespace PinkyPi {
    
    class Light {
    public:
        enum LightType {
            kPointLight,
            kSpotLight,
            kDirectionalLight,
            kMeshLight
        };
        
    public:
        Light();
        ~Light();
        
        std::string name;
        Color color;
        PPFloat intensity;
        LightType lightType;
        
        struct Spot {
            PPFloat innerConeAngle;
            PPFloat outerConeAngle;
            
            Spot(PPFloat inner, PPFloat outer):
                innerConeAngle(inner),
                outerConeAngle(outer)
            {}
        } spot;
    };
}

#endif
