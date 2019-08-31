#ifndef PINKYPI_NODE_H
#define PINKYPI_NODE_H

#include <string>
#include <vector>
#include <memory>
#include "pptypes.h"

namespace PinkyPi {
    
    class Camera;
    class Mesh;
    class Light;
    
    class Node {
    public:
        enum ContentType {
            kContentTypeEmpty,
            kContentTypeCamera,
            kContentTypeMesh,
            kContentTypeLight
        };
        
    public:
        Node();
        ~Node();
        
        std::string name;
        
        std::vector<int> children;
        union {
            Camera *camera;
            Mesh *mesh;
            Light *light;
        };
        Matrix4 transform;
        
        ContentType contentType;
        
//        Matrix4 matrix;
//        Quaterion rotation;
//        Vector3 scale;
//        Vector3 translate;
    };
}

#endif
