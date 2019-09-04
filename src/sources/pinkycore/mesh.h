#ifndef PINKYPI_MESH_H
#define PINKYPI_MESH_H

#include <string>
#include <vector>
#include <memory>
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
        struct Attributes {
            Vector3 normal;
            Vector4 tangent;
            Vector3 uv0;
            Vector3 uv1;
            Color color;
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
            Cluster(int numverts, int numtris);
            ~Cluster();
            
            std::vector<Vector3> vertices;
            std::vector<Attributes> attributes;
            std::vector<Triangle> triangles;
            Material *material;
            
            PPFloat area;
            AABB bounds;
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
