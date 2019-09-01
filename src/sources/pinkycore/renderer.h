#ifndef PINKYPI_RENDERER_H
#define PINKYPI_RENDERER_H

#include <vector>
#include "random.h"
#include "ray.h"

namespace PinkyPi {
    
    class Scene;
    class Config;
    class FrameBuffer;
    
    class Renderer {
    private:
        int samplesPerPixel;
        int pixelSubSamples;
        
        int minDepth;
        int maxDepth;
        float minRussianRouletteCutOff;
        
        struct Context {
            Random random;
        };
        
        std::vector<Context> renderContexts;
        int numMaxJobs;
        
        struct RenderResult {
            Color radiance;
            
            int depth;
            
            Color directRadiance;
            Color indirectRadiance;
            
            //
            Vector3 firstNormal;
            Color firstAlbedo;
            int firstHitMeshId;
            int firstHitClusterId;
            int firstHitPrimitiveId;
            
            Vector3 firstDiffuseNormal;
            Color firstDiffuseAlbedo;
            
            void clear();
        };
        
    public:
        FrameBuffer *framebuffer;
        const Scene *scene;
        
    public:
        Renderer(const Config& config, const Scene* scn);
        ~Renderer();
        
        void render();
        void pathtrace(const Ray& ray, const Scene* scn, Context* cntx, RenderResult *result);
        
        static void renderJob(int jobid, int workerid, void* dat);
    };
}

#endif
