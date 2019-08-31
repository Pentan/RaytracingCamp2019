#ifndef PINKYPI_MESH_H
#define PINKYPI_MESH_H

#include <string>
#include <vector>
#include <memory>
#include "pptypes.h"

namespace PinkyPi {
    
    class Material;
    
    class Mesh {
    public:
        struct Attributes {
            Vector3 normal;
            Vector3 tangent;
            Vector3 uv0;
            Vector3 uv1;
            Color color;
        };
        
        struct Triangle {
            int a;
            int b;
            int c;
            
            int materialId;
        };
        
        class Cluster {
        public:
            Cluster(int numverts, int numtris);
            ~Cluster();
            
            
            std::vector<Vector3> vertices;
            std::vector<Attributes> attributes;
            std::vector<Triangle> triangles;
            Material *material;
        };
        
    public:
        Mesh();
        ~Mesh();
        
        std::string name;
        std::vector<std::shared_ptr<Cluster> > clusters;
        std::vector<std::shared_ptr<Cluster> > emissiveClusters;
    };
}

#endif
