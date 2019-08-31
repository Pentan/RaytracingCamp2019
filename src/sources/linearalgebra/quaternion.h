#ifndef LINEARALGEBRA_QUATERNION_H
#define LINEARALGEBRA_QUATERNION_H

#include <cstdio>
#include <cmath>
#include <algorithm>
#include "constants.h"

namespace linearalgebra {
    
    template<typename FPType> union Quaternion {
        struct {FPType x, y, z, w;};
        FPType q[4];
        
        Quaternion(): x(0.0), y(0.0), z(0.0), w(1.0) {}
        Quaternion(const FPType ix, const FPType iy, const FPType iz, const FPType iw): x(ix), y(iy), z(iz), w(iw) {}
        
        inline void set(const FPType ix, const FPType iy, const FPType iz, const FPType iw);
        inline void set(const FPType iv[4]);
        
        inline bool normalize(void);
        
        inline Matrix4<FPType> getMatrix(void) const;
        
        inline bool hasRotation(void) const;
        
        static inline Quaternion makeRotation(const FPType rad, const FPType ax, const FPType ay, const FPType az);
    };
}
// implementation
#include "quaternion.inl"

#endif
