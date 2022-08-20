#include <iostream>
#include <vector>
#include <stb/stb_image_write.h>

#include "postprocessor.h"
#include "framebuffer.h"

using namespace PinkyPi;

PostProcessor::PostProcessor() :
    exportGamma(2.2)
{
}

int PostProcessor::init(const FrameBuffer *srcbuf, const std::string path, int tilesize, double gamma) {
    sourceBuffer = srcbuf;
    FrameBuffer* pfb = processedBuffer.get();
    if(pfb == nullptr || pfb->getWidth() != sourceBuffer->getWidth() || pfb->getHeight() != sourceBuffer->getHeight()) {
        pfb = new FrameBuffer(sourceBuffer->getWidth(), sourceBuffer->getHeight(), tilesize);
        processedBuffer = std::unique_ptr<FrameBuffer>(pfb);
    }
    pfb->clear();
    int numjobs = pfb->getNumTiles();
    remainingJobs.store(numjobs);
    savePath = path;
    exportGamma = gamma;
    return numjobs;
}

int PostProcessor::process(int jobid) {
    auto srcfb = sourceBuffer;
    auto dstfb = processedBuffer.get();
    auto dstTile = dstfb->getTile(jobid);
    
    for(int iy = dstTile.starty; iy < dstTile.endy; iy++) {
        for(int ix = dstTile.startx; ix < dstTile.endx; ix++) {
            auto srcCol = srcfb->getColor(ix, iy);
            // do something
            dstfb->setColor(ix, iy, srcCol);
        }
    }
    
    return remainingJobs.fetch_sub(1);
}

bool PostProcessor::writeToFile() {
    auto pfb = processedBuffer.get();
    int w = pfb->getWidth();
    int h = pfb->getHeight();
    
    std::vector<unsigned char> rgb8buf;
    rgb8buf.resize(w * h * 3);
    
    auto encodeTo8byte = [](PPColorType c, double gamma) {
        c = std::max(0.0, std::min(1.0, c));
        c = pow(c, 1.0 / gamma);
        return static_cast<unsigned char>(std::max(0.0, std::min(255.0, c * 256.0)));
    };
    
    for(int iy = 0; iy < h; iy++) {
        for(int ix = 0; ix < w; ix++) {
            int ipxl = (ix + (h - iy - 1) * w) * 3;
            Color col = pfb->getColor(ix, iy);
            rgb8buf[ipxl + 0] = encodeTo8byte(col.r, exportGamma);
            rgb8buf[ipxl + 1] = encodeTo8byte(col.g, exportGamma);
            rgb8buf[ipxl + 2] = encodeTo8byte(col.b, exportGamma);
        }
    }
    
    auto l = savePath.length();
    if(savePath[l-4] == '.' && savePath[l-3] == 'j' && savePath[l-2] == 'p' && savePath[l-1] == 'g') {
        stbi_write_jpg(savePath.c_str(), w, h, 3, rgb8buf.data(), 80);
    } else {
        stbi_write_png(savePath.c_str(), w, h, 3, rgb8buf.data(), 0);
    }
    std::cout << "saved:" << savePath << std::endl;
    
    return true;
}
