//
//  mesh.cpp
//  PinkyCore
//
//  Created by SatoruNAKAJIMA on 2019/08/16.
//

#include "node.h"
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
    return static_cast<int>(attributeCounts[i]);
}


void Mesh::Triangle::initialize(const Vector3& va, const Vector3& vb, const Vector3& vc) {
    pa = va;
    edgeab = vb - va;
    edgeac = vc - va;
    Vector3 n = Vector3::cross(edgeab, edgeac);
    PPFloat nl = n.length();
    normal = n / std::max(1e-8, nl);
    area = nl;
    bound.clear();
    bound.expand(va);
    bound.expand(vb);
    bound.expand(vc);
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
            
            tri.initialize(va, vb, vc);
            tri.sampleBorder = clusterArea + tri.area;
            tri.bound.dataId = icl;
            tri.bound.subDataId = itri;
            
            c->bounds.expand(tri.bound);
            clusterArea += tri.area;
        }
        c->area = clusterArea;
        bounds.expand(c->bounds);
        meshArea += c->area;
    }

    // build BVH ?
}

PPFloat Mesh::intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, MeshIntersection* oisect) const
{
    if(!bounds.isIntersect(ray, nearhit, farhit)) {
        return -1.0;
    }
    
    // blute force -----
    int numCls = static_cast<int>(clusters.size());
    PPFloat mint = -1.0;
    int triId = -1;
    int clusterId = -1;
    for(int icls = 0; icls < numCls; icls++) {
        const Cluster *cls = clusters[icls].get();
        if(!cls->bounds.isIntersect(ray, nearhit, farhit)) {
            continue;
        }
        
        int numTris = static_cast<int>(cls->triangles.size());
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

//
MeshCache::ClusterCache::ClusterCache(Mesh::Cluster* src, int numslice) :
sourceCluster(src)
{
    sliceArea.resize(numslice);
    sliceBounds.resize(numslice);
    cachedVertices.resize(numslice);
    
    wholeTriBounds.resize(sourceCluster->triangles.size());
    for (size_t i = 0; i < wholeTriBounds.size(); i++) {
        wholeTriBounds[i].clear();
        wholeTriBounds[i].dataId = static_cast<int>(i); // triangle index
    }
    wholeBounds.clear();

    for (int islc = 0; islc < numslice; islc++) {
        sliceArea[islc] = 0.0;
        sliceBounds[islc].clear();
        auto& cachevert = cachedVertices[islc];
        cachevert.resize(sourceCluster->vertices.size());

        // initial values
        for (size_t i = 0; i < sourceCluster->vertices.size(); i++) {
            Mesh::Attributes attrs = sourceCluster->attributesAt(static_cast<int>(i));
            cachevert[i].vertex = sourceCluster->vertices[i];
            cachevert[i].normal = *attrs.normal;
            cachevert[i].tangent = *attrs.tangent;
            sliceBounds[islc].expand(cachevert[i].vertex);
        }

        expandWholeTriangleBounds(islc);
        wholeBounds.expand(sliceBounds[islc]);
    }
}

void MeshCache::ClusterCache::clearWholeSliceData() {
    wholeBounds.clear();
    for (auto ite = wholeTriBounds.begin(); ite != wholeTriBounds.end(); ++ite) {
        ite->clear();
    }
}

void MeshCache::ClusterCache::expandWholeTriangleBounds(int sliceid) {
    Mesh::Triangle tmptri;
    const auto& cachedvert = cachedVertices[sliceid];
    auto& area = sliceArea[sliceid];

    area = 0.0;
    for (size_t i = 0; i < sourceCluster->triangles.size(); i++) {
        auto& tri = sourceCluster->triangles[i];
        tmptri.initialize(cachedvert[tri.a].vertex, cachedvert[tri.b].vertex, cachedvert[tri.c].vertex);
        wholeTriBounds[i].expand(tmptri.bound);
        area += tmptri.area;
    }
}

void MeshCache::ClusterCache::createTransformed(int sliceid, const Matrix4& m) {
    auto& cachedvert = cachedVertices[sliceid];
    auto& bnd = sliceBounds[sliceid];
    bnd.clear();
    for(size_t i = 0; i < sourceCluster->vertices.size(); i++) {
        Mesh::Attributes attrs = sourceCluster->attributesAt(static_cast<int>(i));
        cachedvert[i].vertex = Matrix4::transformV3(m, sourceCluster->vertices[i]);
        cachedvert[i].normal = Matrix4::mulV3(m, *attrs.normal);
        Vector3 tmpv3(attrs.tangent->x, attrs.tangent->y, attrs.tangent->z);
        tmpv3 = Matrix4::mulV3(m, tmpv3);
        cachedvert[i].tangent.set(tmpv3.x, tmpv3.y, tmpv3.z, attrs.tangent->w);
        bnd.expand(cachedvert[i].vertex);
    }

    expandWholeTriangleBounds(sliceid);
    wholeBounds.expand(bnd);
}

void MeshCache::ClusterCache::createSkinDeformed(int sliceid, const std::vector<Matrix4>& mplt) {
    auto& cachedvert = cachedVertices[sliceid];
    auto& bnd = sliceBounds[sliceid];
    bnd.clear();
    const int numinfl = sourceCluster->attributeCount(Mesh::AttributeId::kWeights);
    for (size_t i = 0; i < sourceCluster->vertices.size(); i++) {
        Mesh::Attributes attrs = sourceCluster->attributesAt(static_cast<int>(i));
        PPFloat totalWeights = 0.0;

        const auto& sv = sourceCluster->vertices[i];
        const auto& sn = *attrs.normal;
        const Vector3 st(attrs.tangent->x, attrs.tangent->y, attrs.tangent->z);

        Vector3 tmpv;
        Vector3 tmpn;
        Vector3 tmpt;

        for (int iinfl = 0; iinfl < numinfl; iinfl++) {
            const auto& w4 = attrs.weights0[iinfl];
            const auto& j4 = attrs.joints0[iinfl];
            
            tmpv += Matrix4::transformV3(mplt[j4.x], sv) * w4.x;
            tmpv += Matrix4::transformV3(mplt[j4.y], sv) * w4.y;
            tmpv += Matrix4::transformV3(mplt[j4.z], sv) * w4.z;
            tmpv += Matrix4::transformV3(mplt[j4.w], sv) * w4.w;

            tmpn += Matrix4::mulV3(mplt[j4.x], sn) * w4.x;
            tmpn += Matrix4::mulV3(mplt[j4.y], sn) * w4.y;
            tmpn += Matrix4::mulV3(mplt[j4.z], sn) * w4.z;
            tmpn += Matrix4::mulV3(mplt[j4.w], sn) * w4.w;

            tmpt += Matrix4::mulV3(mplt[j4.x], st) * w4.x;
            tmpt += Matrix4::mulV3(mplt[j4.y], st) * w4.y;
            tmpt += Matrix4::mulV3(mplt[j4.z], st) * w4.z;
            tmpt += Matrix4::mulV3(mplt[j4.w], st) * w4.w;

            totalWeights += w4.x + w4.y + w4.z + w4.w;
        }

        totalWeights = 1.0 / totalWeights;
        cachedvert[i].vertex = tmpv * totalWeights;
        cachedvert[i].normal = Vector3::normalized(tmpn * totalWeights);
        tmpt = Vector3::normalized(tmpt * totalWeights);
        cachedvert[i].tangent.set(tmpt.x, tmpt.y, tmpt.z, attrs.tangent->w);

        bnd.expand(cachedvert[i].vertex);
    }

    expandWholeTriangleBounds(sliceid);
    wholeBounds.expand(bnd);
}

PPFloat MeshCache::intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, PPTimeType timerate, MeshIntersection* oisect) const {
    // TODO
    return -1.0;
}
