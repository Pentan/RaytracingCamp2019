//
//  tracablestructure.cpp
//  PinkyCore
//
//  Created by SatoruNAKAJIMA on 2022/08/14.
//

#include "tracablestructure.h"
#include "mesh.h"
#include "skin.h"
#include "node.h"
#include "ray.h"

using namespace PinkyPi;

// StaticMeshStructure
void StaticMeshStructure::initialize(int maxslice) {
    if (ownerNode->animatedFlag == 0) {
        const auto& gm = ownerNode->initialTransform.globalMatrix;
        invGlobalMatrix = Matrix4::inverted(gm, nullptr);
        
        // tight bounds
        globalBounds.clear();
        for(auto ic = mesh->clusters.begin(); ic != mesh->clusters.end(); ++ic) {
            auto* c = ic->get();
            for(auto iv = c->vertices.begin(); iv != c->vertices.end(); ++iv) {
                auto v = Matrix4::transformV3(gm, *iv);
                globalBounds.expand(v);
            }
        }
    }
}

void StaticMeshStructure::clearSlice() {
    if (ownerNode->animatedFlag != 0) {
        globalBounds.clear();
    }
}

void StaticMeshStructure::updateSlice(int sliceId) {
    // update AABB
    if (ownerNode->animatedFlag != 0) {
        const auto& gm = ownerNode->currentTransform.globalMatrix;
        globalBounds.expand(AABB::transformed(mesh->bounds, gm));
    }
}

PPFloat StaticMeshStructure::intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, PPTimeType timerate, MeshIntersection* oisect) const {
    Matrix4 igm;
    
    if(!globalBounds.isIntersect(ray, nearhit, farhit)) {
        return -1.0;
    }

    if (ownerNode->animatedFlag == 0) {
        igm = invGlobalMatrix;
    } else {
        igm = ownerNode->computeGlobalMatrix(timerate);
        igm.invert();
    }

    return mesh->intersection(ray.transformed(igm), nearhit, farhit, oisect);
}

// SkinMeshStructure
void SkinMeshStructure::initialize(int maxslice) {
    if (ownerNode->animatedFlag == 0) {
        invGlobalMatrix = Matrix4::inverted(ownerNode->initialTransform.globalMatrix, nullptr);
    }
    jointMatrices.resize(skin->jointNodes.size());
}

void SkinMeshStructure::clearSlice() {
}

void SkinMeshStructure::updateSlice(int sliceId) {
    // make joint matrix table
    for (size_t ijoint = 0; ijoint < skin->jointNodes.size(); ijoint++) {
        auto* jnode = skin->jointNodes[ijoint];
        auto& ibm = skin->inverseBindMatrices[ijoint];
        jointMatrices[ijoint] = ownerNode->currentInverseGlobal * jnode->currentTransform.globalMatrix * ibm;
    }

    cache->createSkinDeformed(sliceId, jointMatrices);
}

PPFloat SkinMeshStructure::intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, PPTimeType timerate, MeshIntersection* oisect) const {
    Matrix4 igm;
    if (ownerNode->animatedFlag == 0) {
        igm = invGlobalMatrix;
    }
    else {
        igm = ownerNode->computeGlobalMatrix(timerate);
        igm.invert();
    }
    
    
    
    return -1.0;
}
