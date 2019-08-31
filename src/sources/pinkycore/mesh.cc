//
//  mesh.cpp
//  PinkyCore
//
//  Created by SatoruNAKAJIMA on 2019/08/16.
//

#include "mesh.h"

#include "pptypes.h"
using namespace PinkyPi;

Mesh::Cluster::Cluster(int numverts, int numtris):
    material(nullptr)
{
    vertices.resize(numverts);
    attributes.resize(numverts);
    triangles.resize(numtris);
}

Mesh::Cluster::~Cluster() {
}


Mesh::Mesh() {
}

Mesh::~Mesh() {
}
