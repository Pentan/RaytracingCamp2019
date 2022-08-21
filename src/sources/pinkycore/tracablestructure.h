#ifndef PINKYPI_TRACABLESTRUCTURE_H
#define PINKYPI_TRACABLESTRUCTURE_H

#include <memory>
#include <vector>
#include "pptypes.h"
#include "ray.h"
#include "aabb.h"
#include "intersection.h"

namespace PinkyPi {
    class Mesh;
    class MeshCache;
    class Skin;
    class Node;
    
    //
    class TracableStructure {
    public:
        Node* ownerNode;
        Matrix4 invGlobalMatrix;
        AABB globalBounds;

        TracableStructure(Node* owner) : ownerNode(owner) {};
        virtual ~TracableStructure() {}
        
        virtual void initialize(int maxslice) = 0;
        virtual void clearSlice() = 0;
        virtual void updateSlice(int sliceId) = 0;
        virtual PPFloat intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, PPTimeType timerate, MeshIntersection* oisect) const = 0;
    };
    
    //
    class StaticMeshStructure : public TracableStructure {
    public:
        Mesh* mesh;
        std::unique_ptr<MeshCache> cache;

        Matrix4 globalMatrix;
        
        StaticMeshStructure(Node* owner, Mesh* m) : TracableStructure(owner), mesh(m) {};
        ~StaticMeshStructure() {};
        
        void initialize(int maxslice) override;
        void clearSlice() override;
        void updateSlice(int sliceId) override;
        PPFloat intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, PPTimeType timerate, MeshIntersection* oisect) const override;
    };
    
    //
    class SkinMeshStructure : public TracableStructure {
    public:
        Mesh* mesh;
        Skin* skin;
        std::unique_ptr<MeshCache> cache;
        std::vector<Matrix4> jointMatrices;
        
        SkinMeshStructure(Node* owner, Mesh* m, Skin* s) : TracableStructure(owner), mesh(m), skin(s) {};
        ~SkinMeshStructure() {};
        
        void initialize(int maxslice) override;
        void clearSlice() override;
        void updateSlice(int sliceId) override;
        PPFloat intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, PPTimeType timerate, MeshIntersection* oisect) const override;
    };
}

#endif /* PINKYPI_TRACABLESTRUCTURE_H */
