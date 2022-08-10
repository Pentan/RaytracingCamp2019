#include <ctime>
#include <iostream>
#include <sstream>
#include "renderer.h"
#include "config.h"
#include "scene.h"
#include "camera.h"
#include "framebuffer.h"
#include "random.h"
#include "ray.h"
#include "postprocessor.h"

using namespace PinkyPi;

namespace {
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
double getTimeInSeconds() {
    return timeGetTime() / 1000.0;
}
#else
#include <sys/time.h>
double getTimeInSeconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (double)tv.tv_usec * 1e-6;
}
#endif
}

Renderer::Renderer(const Config& config, Scene* scn):
    scene(scn),
    frameBufferIndex(0)
{
    framebuffers.reserve(config.framebufferStockCount);
    for(int i = 0; i < config.framebufferStockCount; i++) {
        auto fb = new FrameBuffer(config.width, config.height, config.tileSize);
        framebuffers.push_back(std::unique_ptr<FrameBuffer>(fb));
    }
    
    samplesPerPixel = config.samplesPerPixel;
    pixelSubSamples = config.pixelSubSamples;
    
    minDepth = config.minDepth;
    maxDepth = config.maxDepth;
    minRussianRouletteCutOff = config.minRussianRouletteCutOff;
    
    renderFrames = config.frames;
    fps = config.framesPerSecond;
    exposureSec = config.exposureSecond;
    exposureSlice = config.exposureSlice;
    
    numMaxJobs = config.maxThreads;
    
    limitSecPerFrame = config.limitSec / config.frames;
    progressIntervalSec = config.progressIntervalSec;
    
    auto extpos = config.outputFile.rfind(".");
    if(extpos > 0) {
        saveNameBase = config.outputFile.substr(0, extpos);
        saveExt = config.outputFile.substr(extpos);
    } else {
        saveNameBase = config.outputFile;
        saveExt = ".jpg";
    }
}

Renderer::~Renderer() {
    cleanupWorkers();
}

void Renderer::renderOneFrame(FrameBuffer* fb, PPTimeType opentime, PPTimeType closetime) {
    // scene setup
    scene->seekTime(opentime, closetime, exposureSlice);
    
    // init contexts
    unsigned long seedbase = time(NULL);
    for(int i = 0; i < numMaxJobs; i++) {
        Context& cntx = renderContexts[i];
        cntx.random.setSeed(seedbase + i * 123456789);
        cntx.framebuffer = fb;
    }
    
    // setup jobs
    int numTiles = fb->getNumTiles();
    {
        std::unique_lock<std::mutex> lock(commandQueueMutex);
        for(int i = 0; i < numTiles; i++) {
            JobCommand cmd;
            cmd.type = CommandType::kRender;
            cmd.render.tileIndex = i;
            commandQueue.push(cmd);
        }
    }
    workerCondition.notify_all();
}

void Renderer::saveFrafmebuffer(FrameBuffer* fb, int frameid) {
    std::stringstream ss;
    ss << saveNameBase << frameid << saveExt;
    auto savepath = ss.str();
    
    PostProcessor pp(fb, savepath);
    pp.process();
    pp.writeToFile();
}

void Renderer::render() {
    
    numMaxJobs = 1; //+++++
    
    if(numMaxJobs == 0) {
        numMaxJobs = std::thread::hardware_concurrency();
    }
    renderContexts.resize(numMaxJobs);
    
    if(numMaxJobs > 1) {
        setupWorkers();
    }
    
    for(int i = 0; i < renderFrames; i++) {
        auto fb = framebuffers[frameBufferIndex].get();
        fb->clear();
        
        PPTimeType t = i / static_cast<PPTimeType>(fps);
        renderOneFrame(fb, t, t + exposureSec);
        
        // wait
        if(numMaxJobs <= 1) {
            // serial exection
            processAllCommands();
        } else {
            //
            if(progressIntervalSec > 0.0) {
                // wait and log output
                long sleepMilliSec = static_cast<long>(std::min(0.1, progressIntervalSec) * 1000.0);
                bool waiting = true;
                double logedtime = 0.0;
                while (waiting) {
                    // sleep
                    std::this_thread::sleep_for(std::chrono::milliseconds(sleepMilliSec));
                    
                    // check
                    bool isprocessing = false;
                    for(auto ite = workerInfos.begin(); ite != workerInfos.end(); ++ite) {
                        if(ite->status == WorkerStatus::kProcessing) {
                            isprocessing = true;
                        }
                    }
                    waiting = isprocessing | !commandQueue.empty();
                    
                    // log
                    double curtime = getTimeInSeconds();
                    if(curtime - logedtime > progressIntervalSec) {
                        // print
                        logedtime = curtime;
                    }
                }
            } else {
                // condition wait
                std::unique_lock<std::mutex> lock(commandQueueMutex);
                watcherCondition.wait(lock, [this]{ return commandQueue.empty(); });
            }
        }
        
        // post process
        
        // save frame
        saveFrafmebuffer(fb, i);
        if(numMaxJobs <= 1) {
            processAllCommands();
        }
        
        // next
        frameBufferIndex = (frameBufferIndex + 1) % framebuffers.size();
    }
    
    cleanupWorkers();
}

void Renderer::pathtrace(const Ray& ray, const Scene* scn, Context* cntx, RenderResult *result)
{
    Random& rng = cntx->random;
    result->clear();
    
    Color throughput(1.0, 1.0, 1.0);
    Color radiance(0.0, 0.0, 0.0);
    bool isHitDiffuse = false;
    
    int depth = 0;
    bool isloop = true;
    while(isloop) {
        SceneIntersection interect;
        PPFloat hitt = scene->intersection(ray, kRayOffset, kFarAway, &interect);
        if(hitt <= 0.0) {
            // TODO background
            radiance.set(0.5, 0.5, 0.5);
            break;
        }
        
        
    }
    
    result->radiance = radiance;
    result->depth = depth;
}

void Renderer::renderJob(int workerid, JobCommand cmd) {
    Context *cntx = &renderContexts[workerid];
    Random& rng = cntx->random;
    const FrameBuffer::Tile& tile = cntx->framebuffer->getTile(cmd.render.tileIndex);
    
    PPFloat subPixelSize = 1.0 / pixelSubSamples;
    Camera *camera = scene->cameras[0];
    
    RenderResult result;
    
    for(int iy = tile.starty; iy < tile.endy; iy++) {
        for(int ix = tile.startx; ix < tile.endx; ix++) {
            PPFloat px = PPFloat(ix);
            PPFloat py = PPFloat(iy);
            int pixelId = tile.getPixelIndex(ix, iy);
            FrameBuffer::Pixel& pixel = cntx->framebuffer->getPixel(pixelId);
            
            for(int issy = 0; issy < pixelSubSamples; issy++) {
                for(int issx = 0; issx < pixelSubSamples; issx++) {
                    PPFloat ssx = PPFloat(issx) * subPixelSize;
                    PPFloat ssy = PPFloat(issy) * subPixelSize;
                    
                    for(int ips = 0; ips < samplesPerPixel; ips++) {
                        PPFloat sx = px + ssx + rng.nextDoubleCO() * subPixelSize;
                        PPFloat sy = py + ssy + rng.nextDoubleCO() * subPixelSize;
                        
                        sx = (sx / cntx->framebuffer->getWidth()) * 2.0 - 1.0;
                        sy = (sy / cntx->framebuffer->getHeight()) * 2.0 - 1.0;
                        
                        Ray ray = camera->getRay(sx, sy);
                        pathtrace(ray, scene, cntx, &result);
                        
                        pixel.accumulate(result.radiance);
                    }
                }
            }
        }
    }
}

void Renderer::postprocessJob(int workerid, JobCommand cmd) {
    
}

void Renderer::saveFileJob(int workerid, JobCommand cmd) {
    
}

void Renderer::startWorker(int workerid, Renderer* rndr) {
    rndr->wokerMain(workerid);
}

void Renderer::wokerMain(int workerid) {
    auto* info = &workerInfos[workerid];
    
    while(true) {
        JobCommand cmd;
        info->status = WorkerStatus::kWaiting;
        info->commandType = CommandType::kNoop;
        {
            std::unique_lock<std::mutex> lock(commandQueueMutex);
            workerCondition.wait(lock, [this]{ return !commandQueue.empty() || stopWorkers; });
            cmd = commandQueue.front();
            commandQueue.pop();
            watcherCondition.notify_all();
        }
        
        if(stopWorkers) break;
        
        info->status = WorkerStatus::kProcessing;
        info->commandType = cmd.type;
        processCommand(workerid, cmd);
    }
    
    info->status = WorkerStatus::kStopped;
    info->commandType = CommandType::kNoop;
}

void Renderer::processCommand(int workerid, JobCommand cmd) {
    switch (cmd.type) {
        case kRender:
            renderJob(workerid, cmd);
            break;
        case kPostprocess:
            postprocessJob(workerid, cmd);
            break;
        case kSaveFile:
            saveFileJob(workerid, cmd);
            break;
        default:
            break;
    }
}

void Renderer::processAllCommands() {
    // for serial exection
    while(!commandQueue.empty()) {
        auto cmd = commandQueue.front();
        commandQueue.pop();
        processCommand(0, cmd);
    }
}

void Renderer::setupWorkers() {
    if(numMaxJobs <= 1) return;
    workerInfos.resize(numMaxJobs);
    stopWorkers = false;
    workerPool.reserve(numMaxJobs);
    for(int i = 0; i < numMaxJobs; i++) {
        workerInfos[i].status = WorkerStatus::kStart;
        workerInfos[i].commandType = CommandType::kNoop;
        workerPool.emplace_back(startWorker, i, this);
    }
}

void Renderer::cleanupWorkers() {
    if(workerPool.size() == 0) {
        return;
    }
    
    // stop threads
    {
        std::unique_lock<std::mutex> lock(commandQueueMutex);
        stopWorkers = true;
    }
    workerCondition.notify_all();
    
    for(size_t i = 0; i < workerPool.size(); i++) {
        workerPool[i].join();
    }
    workerPool.clear();
}

void Renderer::RenderResult::clear() {
    radiance.set(0.0, 0.0, 0.0);
    depth = 0;
    
    directRadiance.set(0.0, 0.0, 0.0);
    indirectRadiance.set(0.0, 0.0, 0.0);
    
    firstNormal.set(0.0, 0.0, 0.0);
    firstAlbedo.set(0.0, 0.0, 0.0);
    
    firstHitMeshId = -1;
    firstHitClusterId = -1;
    firstHitPrimitiveId = -1;
    
    firstDiffuseNormal.set(0.0, 0.0, 0.0);
    firstDiffuseAlbedo.set(0.0, 0.0, 0.0);
}
