#ifndef PINKYPI_SCENE_H
#define PINKYPI_SCENE_H

#include <string>
#include <vector>
#include <memory>
#include "pptypes.h"
#include "ray.h"
#include "mesh.h"

namespace PinkyPi {
    
    class AssetLibrary;
    class Node;
    class Camera;
//    class Mesh;
    class Light;
    
    /////
    struct SceneIntersection {
        int meshId;
        MeshIntersection meshIntersect;
        
        struct Detail {
            Vector3 geometryNormal;
            
            Vector3 barycentricCoord;
            Vector3 shadingNormal;
            Vector4 shadingTangent;
            Vector3 texcoord0;
        };
    };
    
    /////
    class Scene {
    public:
        static Scene* buildDefaultScene();
        
//    private:
        std::vector<Node*> nodes;
        
    public:
        // for trace
        std::vector<Mesh*> meshes;
        std::vector<Light*> lights;
        std::vector<Camera*> cameras;
        
    public:
        Scene();
        
        bool buildForTrace(AssetLibrary *assetlib);
        
        PPFloat intersection(const Ray& ray, PPFloat hitnear, PPFloat hitfar, SceneIntersection *oisect) const;
        
    private:
        void traverseNode(Node *node, Matrix4 gm, AssetLibrary *assetlib);
        
    };
}

#endif
