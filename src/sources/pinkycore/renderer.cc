#include "renderer.h"
#include "config.h"
#include "scene.h"
#include "camera.h"
#include "framebuffer.h"
#include "random.h"
#include "ray.h"

using namespace PinkyPi;

Renderer::Renderer(const Config& config, const Scene* scn):
    scene(scn)
{
    framebuffer = new FrameBuffer(config.width, config.height, config.tileSize);
    
    samplesPerPixel = config.samplesPerPixel;
    pixelSubSamples = config.pixelSubSamples;
    
    minDepth = config.minDepth;
    maxDepth = config.maxDepth;
    minRussianRouletteCutOff = config.minRussianRouletteCutOff;
}

Renderer::~Renderer() {
    delete framebuffer;
}

void Renderer::render() {
    
    framebuffer->clear();
    
    numMaxJobs = 1;
    
    // init contexts
    renderContexts.resize(numMaxJobs);
    unsigned long seedbase = time(NULL);
    for(int i = 0; i < numMaxJobs; i++) {
        Context& cntx = renderContexts[i];
        cntx.random.setSeed(seedbase + i * 123456789);
    }
    
    // setup jobs
    int numTiles = framebuffer->getNumTiles();
    for(int i = 0; i < numTiles; i++) {
        Renderer::renderJob(i, i % numMaxJobs, this);
    }
    
    // wait
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

void Renderer::renderJob(int jobid, int workerid, void* dat) {
    Renderer *rndr = reinterpret_cast<Renderer*>(dat);
    const FrameBuffer::Tile& tile = rndr->framebuffer->getTile(jobid);
    Context *cntx = &rndr->renderContexts[workerid];
    Random& rng = cntx->random;
    
    PPFloat subPixelSize = 1.0 / rndr->pixelSubSamples;
    Camera *camera = rndr->scene->cameras[0];
    
    RenderResult result;
    
    for(int iy = tile.starty; iy < tile.endy; iy++) {
        for(int ix = tile.startx; ix < tile.endx; ix++) {
            PPFloat px = PPFloat(ix);
            PPFloat py = PPFloat(iy);
            int pixelId = tile.getPixelIndex(ix, iy);
            FrameBuffer::Pixel& pixel = rndr->framebuffer->getPixel(pixelId);
            
            for(int issy = 0; issy < rndr->pixelSubSamples; issy++) {
                for(int issx = 0; issx < rndr->pixelSubSamples; issx++) {
                    PPFloat ssx = PPFloat(issx) * subPixelSize;
                    PPFloat ssy = PPFloat(issy) * subPixelSize;
                    
                    for(int ips = 0; ips < rndr->samplesPerPixel; ips++) {
                        PPFloat sx = px + ssx + rng.nextDoubleCO() * subPixelSize;
                        PPFloat sy = py + ssy + rng.nextDoubleCO() * subPixelSize;
                        
                        sx = (sx / rndr->framebuffer->getWidth()) * 2.0 - 1.0;
                        sy = (sy / rndr->framebuffer->getHeight()) * 2.0 - 1.0;
                        
                        Ray ray = camera->getRay(sx, sy);
                        rndr->pathtrace(ray, rndr->scene, cntx, &result);
                        
                        pixel.accumulate(result.radiance);
                    }
                }
            }
        }
    }
    
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
