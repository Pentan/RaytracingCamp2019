#ifndef PINKYPI_SCENE_H
#define PINKYPI_SCENE_H

#include <string>
#include <vector>
#include <memory>

namespace PinkyPi {
    
    class Node;
    class Camera;
    class Mesh;
    class Light;
    
    class Scene {
    public:
        static Scene* buildDefaultScene();
        
//    private:
        std::vector<Node*> nodes;
        
        // for trace
        std::vector<Mesh*> meshes;
        std::vector<Light*> lights;
        std::vector<Camera*> cameras;
        
    public:
        Scene();
        
        bool buildForTrace();
        
    private:
        void updateNodesTransform(Node *node);
        
    };
}

#endif
