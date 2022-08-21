#ifndef PINKYPI_SCENE_H
#define PINKYPI_SCENE_H

#include <string>
#include <vector>
#include <memory>
#include "pptypes.h"
#include "ray.h"
#include "intersection.h"

namespace PinkyPi {
    
    class AssetLibrary;
    class Node;
    class Camera;
    class Mesh;
    class Skin;
    class Light;
    class TracableStructure;
    class Config;
    
    /////
    class Scene {
    public:
        static Scene* buildDefaultScene();
        
//    private:
        AssetLibrary* assetLib;
        std::vector<Node*> topLevelNodes;
        std::vector<Node*> containsNodes;

    public:
        // for trace
        std::vector<Node*> tracables;
        std::vector<Node*> lights;
        std::vector<Node*> cameras;
        
    public:
        Scene(AssetLibrary* al);
        
        bool preprocess(Config* config);
        
        void seekTime(PPTimeType opentime, PPTimeType closetime, int slice, int storeId);
        PPFloat intersection(const Ray& ray, PPFloat hitnear, PPFloat hitfar, PPTimeType timerate, SceneIntersection *oisect) const;
        void computeIntersectionDetail(const Ray& ray, PPTimeType timerate, const SceneIntersection& isect, SceneIntersection::Detail* odetail) const;
        
    private:
        void preprocessTraverse(Node *node, Matrix4 gm, Config* config);
        void buildAccelerationStructure(int storeId);
    };
}

#endif
