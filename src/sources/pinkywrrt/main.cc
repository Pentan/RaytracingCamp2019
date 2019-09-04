#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <cstdio>
#include <string>
#include <thread>
#include <chrono>

#include "r1htypes.h"
#include "scene.h"
#include "renderer.h"
#include "framebuffer.h"
#include "tonemapper.h"
//#include "xmlscene.h"
#include "sceneconverter.h"

#include <pinkycore/config.h>
#include <pinkycore/scene.h>
#include <pinkycore/assetlibrary.h>
#include <pinkypi/sceneloader.h>

//
#include "eduptscene.h"
//

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
static double gettimeofday_sec() {
	return timeGetTime() / 1000.0;
}
#else
#include <sys/time.h>
static double gettimeofday_sec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (double)tv.tv_usec * 1e-6;
}
#endif

///
//#define kProgressOutIntervalSec	30.0
static const char kProgressChars[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ(){}[]<>!#$%&=^~@+*?/_";

///
int main(int argc, char *argv[]) {

	std::cout << "PinkyPi with RrT" << std::endl;

    using namespace r1h;

    double startTime = gettimeofday_sec();

    // renderer setup
    Renderer *render = new Renderer();
    
    std::string configPath = "datas/config.json";
    PinkyPi::Config ppconfig;
    if(!ppconfig.load(configPath)) {
        std::cerr << "config load failed. use default settings." << std::endl;
    }
    
    Renderer::Config conf = render->getConfig();
    conf.width = ppconfig.width;
    conf.height = ppconfig.height;
    conf.samples = ppconfig.samplesPerPixel;
    conf.subSamples = ppconfig.pixelSubSamples;
    conf.minDepth = ppconfig.minDepth;
    conf.maxDepth = ppconfig.maxDepth;
    conf.tileSize = ppconfig.tileSize;
    conf.outputFile = ppconfig.outputFile;
	conf.maxLimitTime = ppconfig.limitSec;
	conf.progressOutInterval = ppconfig.progressIntervalSec;
	conf.renderMode = ppconfig.waitUntilFinish ? r1h::Renderer::kStandard : r1h::Renderer::kTimeLimit;
    
    render->setConfig(conf);

	std::cout << "render mode:" << conf.renderMode << std::endl;
	std::cout << "render limit time:" << conf.maxLimitTime << "[sec]" << std::endl;

    // scene setup
    Scene *scene = new Scene();
	bool loaded = false;
	
    if(ppconfig.inputFile.length() > 0) {
		std::cout << "scene file:" << ppconfig.inputFile << std::endl;
        loaded = LoadAndConvertPinkyPiScene(ppconfig.inputFile, &ppconfig, scene);
        
    } else {
        const Renderer::Config& conf = render->getConfig();
        loaded = EduptScene::load(scene, double(conf.width) / conf.height);
        loaded = true;
    }

    printf("scene loaded [%.4f sec]\n", gettimeofday_sec() - startTime);

	if(loaded) {
        const Renderer::Config &conf = render->getConfig();

		// set tone mapper
		ToneMapper *mapper = new ToneMapper();
        //mapper->setGamma(1.0); //+++++
		
		// render
		double renderStart = gettimeofday_sec();
		render->render(scene, true); // detach
		
		// wait to finish
		int outcount = 0;
		double prevouttime = gettimeofday_sec();
        size_t progcharlen = strlen(kProgressChars);
        
        int numcntx = (int)render->getRecderContextCount();
		do {
			double progress = render->getRenderProgress();
			
			std::this_thread::sleep_for(std::chrono::seconds(1));
			
			double curtime = gettimeofday_sec();
			if(conf.progressOutInterval > 0.0 && curtime - prevouttime > conf.progressOutInterval) {
				// progress output
				char buf[16];
				sprintf(buf, "%03d.bmp", outcount);

				mapper->exportBMP(render->getFrameBuffer(), buf);
                printf("progress image %s saved\n", buf);
				outcount++;
				prevouttime += conf.progressOutInterval;
			}
			
            // print progress log
			printf("%.2lf%%:", progress);
            for(int i = 0; i < numcntx; i++) {
                const Renderer::Context *cntx = render->getRenderContext(i);
                //printf("[%.1lf]", cntx->tileProgress * 100.0);
                size_t progindex = size_t(cntx->tileProgress * progcharlen);
                if(progindex >= progcharlen) {
                    progindex = progcharlen - 1;
                }
                printf("%c", kProgressChars[progindex]);
            }
            printf("    \r");
            fflush(stdout);
            
            // timelimit mode
            if(conf.renderMode == Renderer::kTimeLimit) {
                double pasttime = curtime - startTime;
                if(pasttime > conf.maxLimitTime) {
                    for(int i = 0; i < numcntx; i++) {
                        Renderer::Context *cntx = render->getRenderContext(i);
                        cntx->killRequest = true;
                    }
                }
            }
            
		} while( !render->isFinished() );

		printf("render finished (%.4f sec) [%.4f sec]\n", gettimeofday_sec() - renderStart, gettimeofday_sec() - startTime);
		
		// final image
		Renderer::Config renderConf = render->getConfig();
		const char *finalname = renderConf.outputFile.c_str();//"final.bmp";
		//mapper->exportBMP(render->getFrameBuffer(), finalname);
        mapper->exportPNG(render->getFrameBuffer(), finalname);
        printf("%s saved\n", finalname);

		printf("saved [%.4f sec]\n", gettimeofday_sec() - startTime);
		
		delete mapper;
	}

    // cleaning
    delete render;
    delete scene;

    printf("done [%.4f sec]\n", gettimeofday_sec() - startTime);

    return 0;
}
