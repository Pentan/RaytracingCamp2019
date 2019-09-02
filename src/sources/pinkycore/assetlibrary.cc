
#include "pptypes.h"
#include "assetlibrary.h"
#include "mesh.h"
#include "material.h"
#include "texture.h"
#include "light.h"
#include "node.h"
#include "camera.h"
#include "scene.h"

using namespace PinkyPi;

AssetLibrary::AssetLibrary():
    defaultSceneId(-1)
{}

AssetLibrary::~AssetLibrary() {
    
}

void AssetLibrary::setSourcePath(std::string path) {
    sourcePath = path;
    auto findpos = path.find_last_of("/\\");
    if (findpos != std::string::npos) {
        baseDir = path.substr(0, findpos);
    } else {
        baseDir = "";
    }
}


std::string AssetLibrary::getRelativePath(std::string path) const {
    if(baseDir.length() > 0) {
        return baseDir + path;
    }
    return path;
}
