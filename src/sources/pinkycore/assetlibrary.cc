
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
