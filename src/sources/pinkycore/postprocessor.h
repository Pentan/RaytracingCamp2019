#ifndef PINKYPI_POSTPROCESSOR_H
#define PINKYPI_POSTPROCESSOR_H

#include <string>

namespace PinkyPi {
    
    class FrameBuffer;
    
    class PostProcessor {
    public:
        PostProcessor(const FrameBuffer *srcbuf);
        
        void process();
        bool writeToFile(const std::string path);
    };
}

#endif
