#include <cstdio>
#include <iostream>

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
    config.load(configPath);
    
    PinkyPi::Scene *scene = nullptr;
    PinkyPi::SceneLoader loader;
    if(loader.load(config.inputFile)) {
        scene = loader.build();
    } else {
        scene = PinkyPi::Scene::buildDefaultScene();
    }
    
    if(scene == nullptr) {
        std::cerr << "scene is nullptr" << std::endl;
        return 0;
    }
    
    PinkyPi::Renderer renderer(config);
    renderer.render(scene);
    
    PinkyPi::PostProcessor pp(renderer.framebuffer);
    pp.process();
    if(!pp.writeToFile(config.outputFile)) {
        std::cerr << "render result write failed:" << config.outputFile << std::endl;
    }
    
    delete scene;
    
    return 0;
}
