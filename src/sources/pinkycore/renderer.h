#ifndef PINKYPI_RENDERER_H
#define PINKYPI_RENDERER_H

#include <vector>
#include <memory>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "random.h"
#include "ray.h"

namespace PinkyPi {
    
    class Scene;
    class Config;
    class FrameBuffer;
    class PostProcessor;
    
    class Renderer {
    private:
        int samplesPerPixel;
        int pixelSubSamples;
        
        int minDepth;
        int maxDepth;
        float minRussianRouletteCutOff;
        
        int renderFrames;
        double fps;
        double exposureSec;
        int exposureSlice;
        std::string saveNameBase;
        std::string saveExt;
        
        double limitSecPerFrame;
        double progressIntervalSec;
        
        struct Context {
            Random random;
            FrameBuffer* framebuffer;
            PostProcessor* postprocessor;
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
        
        enum CommandType {
            kNoop,
            kRender,
            kPostprocess,
            kSaveFile,
        };
        
        struct JobCommand {
            CommandType type;
            int jobid;
            
            union {
                struct {
                    int tileIndex;
                } render;
            };
        };
        
        enum WorkerStatus {
            kStart,
            kWaiting,
            kProcessing,
            kStopped
        };
        
        struct WorkerInfo {
            CommandType commandType;
            WorkerStatus status;
        };
        
    public:
        std::vector<std::unique_ptr<FrameBuffer> > framebuffers;
        std::vector<std::unique_ptr<PostProcessor> > postprocessors;
        Scene *scene;
        
    public:
        Renderer(const Config& config, Scene* scn);
        ~Renderer();
        
        void render();
        void pathtrace(const Ray& ray, const Scene* scn, Context* cntx, RenderResult *result);
        
        static void startWorker(int workerid, Renderer* rndr);
        void wokerMain(int workerid);
        void processCommand(int workerid, JobCommand cmd);
        
        void renderJob(int workerid, JobCommand cmd);
        void postprocessJob(int workerid, JobCommand cmd);
        void saveFileJob(int workerid, JobCommand cmd);
    private:
        
        size_t frameBufferIndex;
        // FrameBuffer* currentFramebuffer;
        std::vector<std::thread> workerPool;
        std::vector<WorkerInfo> workerInfos;
        std::queue<JobCommand> commandQueue;
        std::queue<JobCommand> interruptQueue;
        std::mutex commandQueueMutex;
        std::condition_variable workerCondition;
        std::condition_variable watcherCondition;
        bool stopWorkers;
        
        void renderOneFrame(FrameBuffer* fb, PostProcessor* pp, PPTimeType opentime, PPTimeType closetime);
        void saveFrafmebuffer(FrameBuffer* fb, int frameid);
        void processAllCommands();
        void setupWorkers();
        void cleanupWorkers();
    };
}

#endif
