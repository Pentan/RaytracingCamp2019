#ifndef PINKYPI_AABB_H
#define PINKYPI_AABB_H

#include "pptypes.h"
#include "ray.h"

namespace PinkyPi {

    class AABB {
    public:
        Vector3 min;
        Vector3 max;
        int dataId;
        
        AABB();
        AABB(const PPFPType minval, const PPFPType maxval);
        AABB(const Vector3 minvec, const Vector3 maxvec);
        
        void clear();

        Vector3 size() const;
        PPFPType surfaceArea() const;
        Vector3 centroid() const;
        
        void expand(const Vector3 &p);
        void expand(const AABB &aabb);
        
        bool isInside(const Vector3 &p) const;
        bool isIntersect(const Ray &ray, PPFPType tnear, PPFPType tfar) const;
    };

}
#endif
