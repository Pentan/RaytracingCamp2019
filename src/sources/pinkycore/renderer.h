#ifndef PINKYPI_RENDERER_H
#define PINKYPI_RENDERER_H

namespace PinkyPi {
    
    class Scene;
    class Config;
    class FrameBuffer;
    
    class Renderer {
    public:
        FrameBuffer *framebuffer;
        
    public:
        Renderer(const Config& config);
        ~Renderer();
        
        void render(const Scene* scene);
    };
}

#endif
