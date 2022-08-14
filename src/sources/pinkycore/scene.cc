#include <cmath>
#include <iostream>

#include "assetlibrary.h"
#include "scene.h"
#include "node.h"
#include "camera.h"
#include "light.h"
#include "mesh.h"
#include "material.h"
#include "tracablestructure.h"

using namespace PinkyPi;

Scene::Scene() {
}

Scene* Scene::buildDefaultScene() {
    return nullptr;
}


void Scene::addNode(Node* node) {
    nodes.push_back(node);
    switch (node->contentType) {
        case Node::kContentTypeMesh:
            if(node->tracable != nullptr) {
                std::cerr << "WARNING node " << node->name << " already has tracable." << std::endl;
                node->tracable.reset();
            }
            if(node->skin == nullptr) {
                auto* trc = new SkinMeshStructure(node->mesh, node->skin);
                node->tracable = std::unique_ptr<TracableStructure>(trc);
            } else {
                auto* trc = new StaticMeshStructure(node->mesh);
                node->tracable = std::unique_ptr<TracableStructure>(trc);
            }
            tracables.push_back(node->tracable.get());
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
    // TODO?
    for(auto i = node->children.begin(); i != node->children.end(); i++) {
        auto child = assetlib->nodes.at(*i).get();
        traverseNode(child, m, assetlib);
    }
}

void Scene::seekTime(PPTimeType opentime, PPTimeType closetime, int slice) {
    
}

PPFloat Scene::intersection(const Ray& ray, PPFloat hitnear, PPFloat hitfar, SceneIntersection *oisect) const {
    MeshIntersection meshisect;
    int meshid = -1;
    
    // TODO object BVH
    
    // blute force -----
    PPFloat mint = -1.0;
    int numMeshes = static_cast<int>(tracables.size());
    for(int i = 0; i < numMeshes; i++) {
        MeshIntersection isect;
        PPFloat t = tracables[i]->intersection(ray, hitnear, hitfar, &isect);
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
