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
        invGlobalMatrix = Matrix4::inverted(ownerNode->initialTransform.globalMatrix, nullptr);
    }
}

void StaticMeshStructure::updateSlice(int sliceId) {
}

PPFloat StaticMeshStructure::intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, PPTimeType timerate, MeshIntersection* oisect) const {
    Matrix4 igm;

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

void SkinMeshStructure::updateSlice(int sliceId) {
    // make joint matrix table
    for (size_t ijoint = 0; ijoint < skin->jointNodes.size(); ijoint++) {
        auto* jnode = skin->jointNodes[ijoint];
        auto& ibm = skin->inverseBindMatrices[ijoint];
        jointMatrices[ijoint] = jnode->currentInverseGlobal * jnode->currentTransform.matrix * ibm;
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
