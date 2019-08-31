#ifndef PINKYPI_SCENE_LOADER_H
#define PINKYPI_SCENE_LOADER_H

namespace PinkyPi {
    
    class Scene;
    
    class SceneLoader {
    public:
        static Scene* load(std::string filepath);
    };
}

#endif
