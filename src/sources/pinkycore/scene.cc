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
#include "animation.h"
#include "keyframesampler.h"
#include "config.h"

using namespace PinkyPi;

Scene::Scene(AssetLibrary* al) :
    assetLib(al)
{
}

Scene* Scene::buildDefaultScene() {
    return nullptr;
}

bool Scene::preprocess(Config* config) {
    // collect nodes
    containsNodes.reserve(assetLib->nodes.size());

    Matrix4 m;
    for(auto i = topLevelNodes.begin(); i != topLevelNodes.end(); i++) {
        auto node = *i;
        preprocessTraverse(node, m, config);
    }
    
    backgroundTexture = assetLib->backgroundTex.get();
    
    return true;
}

void Scene::preprocessTraverse(Node *node, Matrix4 gm, Config* config) {
    Matrix4 m = gm * node->initialTransform.matrix;
    node->initialTransform.globalMatrix = m;
    node->currentTransform = node->initialTransform;
    node->transformCache.resize(config->exposureSlice);
    containsNodes.push_back(node);
    
    switch (node->contentType) {
        case Node::kContentTypeMesh:
            node->content.mesh->preprocess();
            if(node->tracable != nullptr) {
                std::cerr << "WARNING node " << node->name << " already has tracable." << std::endl;
                node->tracable.reset();
            }
            if(node->content.skin != nullptr) {
                auto* trc = new SkinMeshStructure(node, node->content.mesh, node->content.skin);
                trc->initialize(config->exposureSlice);
                node->tracable = std::unique_ptr<TracableStructure>(trc);
            } else {
                auto* trc = new StaticMeshStructure(node, node->content.mesh);
                trc->initialize(config->exposureSlice);
                node->tracable = std::unique_ptr<TracableStructure>(trc);
            }
            tracables.push_back(node);
            break;
        case Node::kContentTypeCamera:
            cameras.push_back(node);
            break;
        case Node::kContentTypeLight:
            lights.push_back(node);
            break;
        default:
            break;
    }

    for(auto i = node->children.begin(); i != node->children.end(); i++) {
        auto* child = assetLib->nodes.at(*i).get();
        child->parent = node;
        if (node->animatedFlag != 0) {
            child->animatedFlag |= Node::kAnimatedInTree;
        }
        preprocessTraverse(child, m, config);
    }
}

void Scene::seekTime(PPTimeType opentime, PPTimeType closetime, int slice, int storeId) {
    PPTimeType tdiv = static_cast<PPTimeType>(std::max(1, slice - 1)); // [0,1]
    for(int islc = 0; islc < slice; islc++) {
        PPTimeType t = static_cast<PPTimeType>(islc) / tdiv;
        PPTimeType curtime = opentime * (1.0 - t) + closetime * t;
        
        // update node transform
        for(auto ianim = assetLib->animations.begin(); ianim != assetLib->animations.end(); ++ianim) {
            auto* anim = ianim->get();
            for(auto itarget = anim->targets.begin(); itarget != anim->targets.end(); ++itarget) {
                auto& target = *itarget;
                switch(target.targetProp) {
                    case Animation::TargetProperty::kTranslation:
                        target.node->currentTransform.translate = target.sampler->sampleVector3(curtime);
                        target.node->isTransformDirty = true;
                        break;
                    case Animation::TargetProperty::kRotation:
                        target.node->currentTransform.rotation = target.sampler->sampleQuaternion(curtime);
                        target.node->isTransformDirty = true;
                        break;
                    case Animation::TargetProperty::kScale:
                        target.node->currentTransform.scale = target.sampler->sampleVector3(curtime);
                        target.node->isTransformDirty = true;
                        break;
                    case Animation::TargetProperty::kMorphWeights:
                        // TODO
                        break;
                }
            }
        }
        
        // make global transform
        for(auto inode = containsNodes.begin(); inode != containsNodes.end(); ++inode) {
            auto* node = *inode;

            if(node->isTransformDirty) {
                node->currentTransform.makeMatrix();
                node->isTransformDirty = false;
            }
            if(node->parent != nullptr) {
                node->currentTransform.globalMatrix = node->parent->currentTransform.globalMatrix * node->currentTransform.matrix;
            } else {
                node->currentTransform.globalMatrix = node->currentTransform.matrix;
            }
            node->currentInverseGlobal = Matrix4::inverted(node->currentTransform.globalMatrix, nullptr);
            node->transformCache[islc] = node->currentTransform;
        }

        // update traceables
        for (auto ite = tracables.begin(); ite != tracables.end(); ++ite) {
            auto* nd = *ite;
            auto* trc = nd->tracable.get();
            trc->updateSlice(islc);
        }
    }
    
    // build AS
    buildAccelerationStructure(storeId);
}

PPFloat Scene::intersection(const Ray& ray, PPFloat hitnear, PPFloat hitfar, PPTimeType timerate, SceneIntersection *oisect) const {
    MeshIntersection meshisect;
    int meshid = -1;
    
    // TODO object BVH
    
    // blute force -----
    PPFloat mint = -1.0;
    int numMeshes = static_cast<int>(tracables.size());
    for(int i = 0; i < numMeshes; i++) {
        MeshIntersection isect;
        auto* trc = tracables[i]->tracable.get();
        PPFloat t = trc->intersection(ray, hitnear, hitfar, timerate, &isect);
        if(t > 0.0) {
            if(mint > t || mint < 0.0) {
                mint = t;
                meshisect = isect;
                meshid = trc->mesh->assetId;
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

void Scene::computeIntersectionDetail(const Ray& ray, PPFloat hitt, PPTimeType timerate, const SceneIntersection& isect, IntersectionDetail* odetail) const {
    auto* trc = tracables[isect.meshId]->tracable.get();
    trc->intersectionDetail(ray, hitt, timerate, isect.meshIntersect, odetail);
}

void Scene::buildAccelerationStructure(int storeId) {
    (void)storeId; // TODO? multi buffering
    
    // build local AS
    for (auto ite = tracables.begin(); ite != tracables.end(); ++ite) {
        (*ite)->tracable->updateFinished();
    }

    // build AS
}
