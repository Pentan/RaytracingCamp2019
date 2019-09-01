#include <string>
#include <sstream>

#include <catch2/catch.hpp>
#include "../testsupport.h"

#include <pinkycore/pptypes.h>
#include <pinkycore/assetlibrary.h>
#include <pinkycore/scene.h>

#include <pinkypi/sceneloader.h>

using namespace PinkyPi;

TEST_CASE("Scene loader test", "[SceneLoader]") {
    std::string basedir = "/models";
//    std::string filename = "/BoxTexturedNonPowerOfTwo/glTF/BoxTexturedNonPowerOfTwo.gltf";
//    std::string filename = "/Lantern/glTF/Lantern.gltf";
    std::string filename = "/BlenderExport/box01.gltf";
    
    std::stringstream ss;
    ss <<  PINKYPI_TEST_DATA_DIR << basedir << filename;
    std::string gltfpath = ss.str();
    
    AssetLibrary *assetlib = SceneLoader::load(gltfpath);
    Scene *scene = assetlib->getDefaultScene();
    
    REQUIRE(scene != nullptr);
}
