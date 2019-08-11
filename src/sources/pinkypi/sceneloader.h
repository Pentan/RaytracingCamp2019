#ifndef PINKYPI_SCENE_LOADER_H
#define PINKYPI_SCENE_LOADER_H

namespace PinkyPi {
    
    class Scene;
    
    class SceneLoader {
    public:
        SceneLoader();

        bool load(std::string filepath);
        Scene* build();
    };
}

#endif
