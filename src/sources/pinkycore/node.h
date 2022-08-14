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
    class TracableStructure;
    
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
                union {
                    Skin *skin;
                    int skinId;
                };
            };
            Light *light;
        };
        
        ContentType contentType;
        
        struct Transform {
            Matrix4 matrix;
            Quaterion rotation;
            Vector3 scale;
            Vector3 translate;
            
            void makeMatrix();
        };
        
        Transform initialTransform;
        Transform currentTransform;
        std::vector<Transform> transformCache;
        std::unique_ptr<TracableStructure> tracable;
    };
}

#endif
