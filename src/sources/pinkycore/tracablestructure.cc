//
//  tracablestructure.cpp
//  PinkyCore
//
//  Created by SatoruNAKAJIMA on 2022/08/14.
//

#include "tracablestructure.h"
#include "mesh.h"
#include "skin.h"

using namespace PinkyPi;

// StaticMeshStructure
void StaticMeshStructure::initialize() {
}

void StaticMeshStructure::update(PPTimeType starttime, PPTimeType endtime) {
}

PPFloat StaticMeshStructure::intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, MeshIntersection* oisect) const {
    return mesh->intersection(ray, nearhit, farhit, oisect);
}

// SkinMeshStructure
void SkinMeshStructure::initialize() {
}

void SkinMeshStructure::update(PPTimeType starttime, PPTimeType endtime) {
}

PPFloat SkinMeshStructure::intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, MeshIntersection* oisect) const {
    
}
