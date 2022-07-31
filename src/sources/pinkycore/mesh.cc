//
//  mesh.cpp
//  PinkyCore
//
//  Created by SatoruNAKAJIMA on 2019/08/16.
//

#include "mesh.h"
#include "bvh.h"
#include "pptypes.h"

using namespace PinkyPi;

Mesh::Cluster::Cluster(int numverts, int numtris, const std::map<AttributeId, int>& attrdesc):
    material(nullptr)
{
    vertices.resize(numverts);
    triangles.resize(numtris);

    const size_t attrSizeTbl[] = {
        sizeof(Vector3),    // kNormal,
        sizeof(Vector4),    // kTangent,
        sizeof(Vector3),    // kUv,
        sizeof(Vector4),    // kColor,
        sizeof(IntVec4),    // kJoints,
        sizeof(Vector4),    // kWeights
    };

    attributeDataSize = 0;
    memset(&attributeCounts, 0, sizeof(attributeCounts));

    for (auto kv : attrdesc) {
        attributeCounts[kv.first] = kv.second;
        attributeDataSize += attrSizeTbl[kv.second];
    }

    attributeBuffer.resize(numverts * attributeDataSize);

    attributeOffsets[0] = 0;
    for (int i = 1; i < kNumAttrs; i++) {
        int k = i - 1;
        attributeOffsets[i] = attributeOffsets[k] + attrSizeTbl[k] * attributeCounts[k];
    }
}

Mesh::Cluster::~Cluster() {
}

Mesh::Attributes Mesh::Cluster::attributesAt(int i) {
    Attributes attrs;
    unsigned char* data = attributeBuffer.data() + attributeDataSize * i;

    attrs.normal = reinterpret_cast<Vector3*>(data + attributeOffsets[kNormal]);
    attrs.tangent = reinterpret_cast<Vector4*>(data + attributeOffsets[kTangent]);
    attrs.uv0 = reinterpret_cast<Vector3*>(data + attributeOffsets[kUv]);
    attrs.color0 = reinterpret_cast<Vector4*>(data + attributeOffsets[kColor]);
    attrs.joints0 = reinterpret_cast<IntVec4*>(data + attributeOffsets[kJoints]);
    attrs.weights0 = reinterpret_cast<Vector4*>(data + attributeOffsets[kNormal]);

    return attrs;
}

int Mesh::Cluster::attributeCount(AttributeId i) const {
    return attributeCounts[i];
}

PPFloat Mesh::Triangle::intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, PPFloat *obb, PPFloat *obc) const
{
    Vector3 r = ray.origin - pa;
    Vector3 u = Vector3::cross(ray.direction, edgeac);
    Vector3 v = Vector3::cross(r, edgeab);
    
    PPFloat div = 1.0 / Vector3::dot(u, edgeab);
    PPFloat t = Vector3::dot(v, edgeac) * div;
    PPFloat b = Vector3::dot(u, r) * div;
    PPFloat c = Vector3::dot(v, ray.direction) * div;
    
    if((b < 0.0) || (c < 0.0) || (b + c > 1.0) || (t < nearhit) || (t > farhit)) {
        return -1.0;
    }
    
    if(obb != nullptr) { *obb = b; }
    if(obc != nullptr) { *obc = c; }
    return t;
}

Mesh::Mesh():
    triangleBVH(nullptr)
{
}

Mesh::~Mesh() {
    delete triangleBVH;
}

void Mesh::setGlobalTransform(const Matrix4 &m) {
    globalTransform = m;
    invGlobalTransform = Matrix4::inverted(m, nullptr);
    invTransGlobalTransform = invGlobalTransform;
    invTransGlobalTransform.transpose();
}

void Mesh::preprocess() {
    
    bounds.clear();
    triangleBVH = new BVH(totalTriangles);
    
    // int triangles
    PPFloat meshArea = 0.0;
    for(int icl = 0; icl < clusters.size(); icl++) {
        auto c = clusters[icl].get();
        c->bounds.clear();
        
        PPFloat clusterArea = 0.0;
        for(int itri = 0; itri < c->triangles.size(); itri++) {
            Triangle& tri = c->triangles[itri];
            
            const Vector3& va = c->vertices.at(tri.a);
            const Vector3& vb = c->vertices.at(tri.b);
            const Vector3& vc = c->vertices.at(tri.c);
            
            tri.pa = va;
            tri.edgeab = vb - va;
            tri.edgeac = vc - va;
            Vector3 n = Vector3::cross(tri.edgeab, tri.edgeac);
            PPFloat nl = n.length();
            tri.normal = n / nl;
            tri.area =  nl;
            tri.sampleBorder = clusterArea + tri.area;
            tri.bound.clear();
            tri.bound.expand(va);
            tri.bound.expand(vb);
            tri.bound.expand(vc);
            
            tri.bound.dataId = icl;
            tri.bound.subDataId = itri;
            
            c->bounds.expand(va);
            c->bounds.expand(vb);
            c->bounds.expand(vc);
            
            bounds.expand(va);
            bounds.expand(vb);
            bounds.expand(vc);
            
            clusterArea += tri.area;
        }
        c->area = clusterArea;
        meshArea += c->area;
    }
}

PPFloat Mesh::intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, MeshIntersection* oisect) const
{
    if(!bounds.isIntersect(ray, nearhit, farhit)) {
        return -1.0;
    }
    
    // blute force -----
    int numCls = clusters.size();
    PPFloat mint = -1.0;
    int triId = -1;
    int clusterId = -1;
    for(int icls = 0; icls < numCls; icls++) {
        const Cluster *cls = clusters[icls].get();
        if(!cls->bounds.isIntersect(ray, nearhit, farhit)) {
            continue;
        }
        
        int numTris = cls->triangles.size();
        for(int itri = 0; itri < numTris; itri++) {
            const Triangle& tri = cls->triangles[itri];
            PPFloat thit = tri.intersection(ray, nearhit, farhit, nullptr, nullptr);
            if(thit > 0.0) {
                if(mint > thit || mint < 0.0) {
                    mint = thit;
                    triId = itri;
                    clusterId = icls;
                }
            }
        }
    }
    //-----
    
    if(oisect != nullptr) {
        oisect->clusterId = clusterId;
        oisect->triangleId = triId;
    }
    
    return mint;
}
