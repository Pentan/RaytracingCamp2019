#ifndef PINKYPI_POSTPROCESSOR_H
#define PINKYPI_POSTPROCESSOR_H

#include <string>
#include <atomic>
#include <memory>

namespace PinkyPi {
    
    class FrameBuffer;
    
    class PostProcessor {
    public:
        const FrameBuffer* sourceBuffer;
        std::unique_ptr<FrameBuffer> processedBuffer;
        std::atomic<int> remainingJobs;
        std::string savePath;
        double exportGamma;
        
        PostProcessor();
        
        int init(const FrameBuffer *srcbuf, const std::string path, int tilesize, double gamma);
        int process(int jobid);
        bool writeToFile();
    };
}

#endif
