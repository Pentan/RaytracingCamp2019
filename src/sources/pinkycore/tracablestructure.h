//
//  tracablestructure.hpp
//  PinkyCore
//
//  Created by SatoruNAKAJIMA on 2022/08/14.
//

#ifndef PINKYPI_TRACABLESTRUCTURE_H
#define PINKYPI_TRACABLESTRUCTURE_H

#include "pptypes.h"
#include "ray.h"
#include "aabb.h"
#include "intersection.h"

namespace PinkyPi {
    class Mesh;
    class MeshCache;
    class Skin;
    
    //
    class TracableStructure {
    public:
        TracableStructure() {}
        virtual ~TracableStructure() {}
        
        virtual void initialize() = 0;
        virtual void update(PPTimeType starttime, PPTimeType endtime) = 0;
        virtual PPFloat intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, MeshIntersection* oisect) const = 0;
    };
    
    //
    class StaticMeshStructure : public TracableStructure {
    public:
        Mesh* mesh;
        
        StaticMeshStructure(Mesh* m) : mesh(m) {};
        ~StaticMeshStructure() {};
        
        void initialize() override;
        void update(PPTimeType starttime, PPTimeType endtime) override;
        PPFloat intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, MeshIntersection* oisect) const override;
    };
    
    //
    class SkinMeshStructure : public TracableStructure {
    public:
        Mesh* mesh;
        Skin* skin;
        MeshCache* cache;
        
        SkinMeshStructure(Mesh* m, Skin* s) : mesh(m), skin(s) {};
        ~SkinMeshStructure() {};
        
        void initialize() override;
        void update(PPTimeType starttime, PPTimeType endtime) override;
        PPFloat intersection(const Ray& ray, PPFloat nearhit, PPFloat farhit, MeshIntersection* oisect) const override;
    };
}

#endif /* PINKYPI_TRACABLESTRUCTURE_H */
