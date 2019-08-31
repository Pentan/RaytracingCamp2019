#ifndef PINKYPI_TEXTURE_H
#define PINKYPI_TEXTURE_H

#include <string>
#include <vector>
#include <memory>
#include <cmath>

#include <pinkycore/pptypes.h>

namespace PinkyPi {
    
    // sample
    struct TexcelSample {
        Color rgb;
        PPColorType a;
        
        TexcelSample():
            rgb(0.0),
            a(1.0)
        {}
        
        void powRGB(PPColorType x) {
            rgb.r = std::pow(rgb.r, x);
            rgb.g = std::pow(rgb.g, x);
            rgb.b = std::pow(rgb.b, x);
        }
    };
    
    // Base class
    class Texture {
    public:
        Texture() {};
        virtual ~Texture() {};
        
        virtual TexcelSample sample(PPFloat x, PPFloat y) const = 0;
        
        std::string name;
    };
    
    // Image texture
    class ImageTexture : public Texture {
    public:
        enum SampleType {
            kNearest,
            kLinear
        };
        enum WrapType {
            kClamp,
            kRepeat
        };
        
    public:
        ImageTexture(int w, int h);
        virtual ~ImageTexture();
        
        virtual TexcelSample sample(PPFloat x, PPFloat y) const;
        
        void fillColor(const Color rgb, PPColorType a, double gamma);
        
        void initWith8BPPImage(const unsigned char *src, int comps, double gamma);
        void initWith16BPPImage(const unsigned short *src, int comps, double gamma);
        void initWithFpImage(const float *src, int comps, double gamma);
        
    public:
        int width;
        int height;
        bool hasAlpha;
        
        SampleType sampleType;
        WrapType wrapX;
        WrapType wrapY;
        
    private:
        TexcelSample *image;
        
        int wrapSampleX(int x) const;
        int wrapSampleY(int y) const;
    };
}

#endif
