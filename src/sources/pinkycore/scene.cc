#include <cmath>

#include "assetlibrary.h"
#include "scene.h"
#include "node.h"
#include "camera.h"
#include "light.h"
#include "mesh.h"
#include "material.h"

using namespace PinkyPi;

Scene::Scene() {
}

Scene* Scene::buildDefaultScene() {
    return nullptr;
}

bool Scene::buildForTrace(AssetLibrary *assetlib) {
    // traverse nodes
    Matrix4 m;
    for(auto i = nodes.begin(); i != nodes.end(); i++) {
        auto node = *i;
        traverseNode(node, m, assetlib);
    }
    
    
    return true;
}

void Scene::traverseNode(Node *node, Matrix4 gm, AssetLibrary *assetlib) {
    Matrix4 m = gm * node->currentTransform.matrix;
    for(auto i = node->children.begin(); i != node->children.end(); i++) {
        auto child = assetlib->nodes.at(*i).get();
        traverseNode(child, m, assetlib);
    }
    
    switch (node->contentType) {
        case Node::kContentTypeMesh:
            meshes.push_back(node->mesh);
            break;
        case Node::kContentTypeCamera:
            cameras.push_back(node->camera);
            break;
        case Node::kContentTypeLight:
            lights.push_back(node->light);
            break;
        default:
            break;
    }
}

void Scene::seekTime(PPTimeType opentime, PPTimeType closetime, int slice) {
    
}

PPFloat Scene::intersection(const Ray& ray, PPFloat hitnear, PPFloat hitfar, SceneIntersection *oisect) const {
    MeshIntersection meshisect;
    int meshid = -1;
    
    // blute force -----
    PPFloat mint = -1.0;
    int numMeshes = static_cast<int>(meshes.size());
    for(int i = 0; i < numMeshes; i++) {
        MeshIntersection isect;
        PPFloat t = meshes[i]->intersection(ray, hitnear, hitfar, &isect);
        if(t > 0.0) {
            if(mint > t || mint < 0.0) {
                mint = t;
                meshisect = isect;
                meshid = i;
            }
        }
    }
    //-----
    
    if(mint > 0.0 && oisect != nullptr) {
        oisect->meshId = meshid;
        oisect->meshIntersect = meshisect;
    }
    
    return mint;
}
