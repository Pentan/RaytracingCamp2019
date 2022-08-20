#ifndef PINKYPI_MESH_H
#define PINKYPI_MESH_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "pptypes.h"
#include "ray.h"
#include "intersection.h"
#include "aabb.h"
#include "tracablestructure.h"

namespace PinkyPi {
    
    /////
    class Material;
    class BVH;
    class MeshCache;
    
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
            
            void initialize(const Vector3& va, const Vector3& vb, const Vector3& vc);
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
    
    //
    class MeshCache {
    public:
        class ClusterCache {
        public:
            struct CachedAttribute {
                Vector3 vertex;
                Vector3 normal;
                Vector4 tangent;
            };
            
            ClusterCache(Mesh::Cluster* src, int numslice);
            void clearWholeSliceData();
            void expandWholeTriangleBounds(int sliceid);
            void createTransformed(int sliceid, const Matrix4& m);
            void createSkinDeformed(int sliceid, const std::vector<Matrix4>& mplt);
            
            Mesh::Cluster* sourceCluster;
            // per slice data
            std::vector<std::vector<CachedAttribute> > cachedVertices;
            std::vector<PPFloat> sliceArea;
            std::vector<AABB> sliceBounds;

            // whole slice data
            AABB wholeBounds;
            std::vector<AABB> wholeTriBounds;
        };

        Mesh* mesh;
        std::vector<std::unique_ptr<ClusterCache> > clusterCaches;

        MeshCache(Mesh* m) : mesh(m) {}
        ~MeshCache() {}

        void createSkinDeformed(int sliceid, const std::vector<Matrix4>& mplt) {
            for (auto ite = clusterCaches.begin(); ite != clusterCaches.end(); ++ite) {
                ite->get()->createSkinDeformed(sliceid, mplt);
            }
        }

        PPFloat intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, PPTimeType timerate, MeshIntersection* oisect) const;
    };
}

#endif
