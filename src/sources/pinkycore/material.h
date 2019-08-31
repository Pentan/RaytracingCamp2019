#ifndef PINKYPI_METARIAL_H
#define PINKYPI_METARIAL_H

#include <string>
#include <vector>
#include <memory>

#include "pptypes.h"

namespace PinkyPi {
    
    class Texture;
    
    class Material {
    public:
        enum AlphaMode {
            kAlphaAsOpaque,
            kAlphaAsMask,
            kAlphaAsBlend
        };
        
        struct TextureInfo {
            Texture *texture;
            int texCoord;
            PPFloat scale; // for normal and occulution
            
            TextureInfo():
                texture(nullptr),
                texCoord(0),
                scale(1.0)
            {}
        };
        
    public:
        Material();
        ~Material();
        
        std::string name;
        
        // pbrMetallicRoughness
        TextureInfo baseColorTexture;
        Color baseColorFactor;
        PPColorType baseColorAlpha;
        TextureInfo metallicRoughnessTexture;
        PPFloat metallicFactor;
        PPFloat roughnessFactor;
        
        // Additions
        TextureInfo normalTexture;
        TextureInfo occlusionTexture;
        TextureInfo emissiveTexture;
        Color emissiveFactor;
        
        AlphaMode alphaMode;
        PPFloat alphaCutoff;
        bool doubleSided;
        
        // Extra
        PPFloat ior;
        // Clear coat factor
        // Shean factor
        // SSS factor
        // ...
    };
}

#endif
