#include <cstdio>
#include <iostream>

#include <pinkycore/assetlibrary.h>
#include <pinkycore/scene.h>
#include <pinkycore/renderer.h>
#include <pinkycore/config.h>
#include <pinkycore/postprocessor.h>

#include "sceneloader.h"

int main(int argc, char* argv[])
{
    std::cout << "Hellooooo Pinky Pi!\n" << std::endl;
    
    std::string configPath = "datas/config.json";
    PinkyPi::Config config;
    if(!config.load(configPath)) {
        std::cerr << "config load failed. use default settings." << std::endl;
    }
    
    
    PinkyPi::AssetLibrary *assetlib = nullptr;
    PinkyPi::Scene *scene = nullptr;
    
    if(config.inputFile.length() > 0) {
        assetlib = PinkyPi::SceneLoader::load(config.inputFile);
        scene = assetlib->getDefaultScene();
    } else {
//        scene = PinkyPi::Scene::buildDefaultScene();
    }
    
    if(scene == nullptr) {
        std::cerr << "scene is nullptr" << std::endl;
        return 0;
    }
    
    scene->buildForTrace(assetlib);
    
    PinkyPi::Renderer renderer(config, scene);
    renderer.render();
    
//    PinkyPi::PostProcessor pp(renderer.framebuffer);
//    pp.process();
//    if(!pp.writeToFile(config.outputFile)) {
//        std::cerr << "render result write failed:" << config.outputFile << std::endl;
//    }
    
    delete scene;
    
    return 0;
}
