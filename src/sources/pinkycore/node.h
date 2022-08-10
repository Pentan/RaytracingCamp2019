#ifndef PINKYPI_NODE_H
#define PINKYPI_NODE_H

#include <string>
#include <vector>
#include <memory>
#include "pptypes.h"

namespace PinkyPi {
    
    class Camera;
    class Mesh;
    class Skin;
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
            struct {
                Mesh *mesh;
                struct {
                    Skin *skin;
                    int skinId;
                };
            };
            Light *light;
        };
        
        ContentType contentType;
        
        struct Transfom {
            Matrix4 matrix;
            Quaterion rotation;
            Vector3 scale;
            Vector3 translate;
            
            void makeMatrix();
        };
        
        Transfom initialTransform;
        Transfom currentTransform;
    };
}

#endif
