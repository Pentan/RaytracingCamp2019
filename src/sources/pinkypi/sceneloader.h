#ifndef PINKYPI_SCENE_LOADER_H
#define PINKYPI_SCENE_LOADER_H

namespace PinkyPi {
    
    class AssetLibrary;
    class Config;
    
    class SceneLoader {
    public:
        static AssetLibrary* load(std::string filepath, const Config *conf);
    };
}

#endif
