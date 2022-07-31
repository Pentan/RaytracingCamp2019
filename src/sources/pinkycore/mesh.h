#ifndef PINKYPI_MESH_H
#define PINKYPI_MESH_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "pptypes.h"
#include "ray.h"
#include "aabb.h"

namespace PinkyPi {
    
    /////
    class Material;
    class BVH;
    
    /////
    struct MeshIntersection {
        int clusterId;
        int triangleId;
    };
    
    /////
    class Mesh {
    public:
        enum AttributeId {
            kNormal,
            kTangent,
            kUv,
            kColor,
            kJoints,
            kWeights,

            kNumAttrs
        };

        struct Attributes {
            Vector3* normal;
            Vector4* tangent;
            Vector3* uv0;
            Vector4* color0;
            IntVec4* joints0;
            Vector4* weights0;
        };
        
        struct Triangle {
            int a;
            int b;
            int c;
            
            int clusterId;
            
            Vector3 pa;
            Vector3 edgeab;
            Vector3 edgeac;
            Vector3 normal;
            PPFloat area;
            PPFloat sampleBorder;
            AABB bound;
            
            PPFloat intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, PPFloat *obb, PPFloat *obc) const;
        };
        
        class Cluster {
        public:
            Cluster(int numverts, int numtris, const std::map<AttributeId, int>& attrdesc);
            ~Cluster();

            Attributes attributesAt(int i);
            int attributeCount(AttributeId i) const;
            
            std::vector<Vector3> vertices;
            std::vector<unsigned char> attributeBuffer;
            std::vector<Triangle> triangles;
            Material *material;
            
            PPFloat area;
            AABB bounds;

            size_t attributeCounts[kNumAttrs];
            size_t attributeOffsets[kNumAttrs];
            size_t attributeDataSize;
        };
        
    public:
        Mesh();
        ~Mesh();
        
        std::string name;
        std::vector<std::shared_ptr<Cluster> > clusters;
        std::vector<std::shared_ptr<Cluster> > emissiveClusters;
        
        int totalVertices;
        int totalTriangles;
        
    public:
        Matrix4 globalTransform;
        Matrix4 invGlobalTransform;
        Matrix4 invTransGlobalTransform;
        
        AABB bounds;
        BVH *triangleBVH;
        
        void setGlobalTransform(const Matrix4 &m);
        
        void preprocess();
        
        PPFloat intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, MeshIntersection* oisect) const;
    };
}

#endif
