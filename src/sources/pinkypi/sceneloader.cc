#include <iostream>
#include <fstream>
#include <cstdio>

#include <nlohmann/json.hpp>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#define TINYGLTF_NO_INCLUDE_JSON
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE
#include <tinygltf/tiny_gltf.h>

#include <pinkycore/scene.h>

#include "sceneloader.h"

using namespace PinkyPi;

SceneLoader::SceneLoader() {
    
}

bool SceneLoader::load(std::string filepath) {
    tinygltf::TinyGLTF gltfloader;
    
    tinygltf::Model model;
    std::string err;
    std::string warn;
    bool loadres;
    
    loadres = gltfloader.LoadASCIIFromFile(&model, &err, &warn, filepath);
    
    if(!loadres) {
        std::cerr << "glTF load err:" << err << std::endl;
        return false;
    }
    
    return true;
}


Scene* SceneLoader::build() {
    return nullptr;
}
