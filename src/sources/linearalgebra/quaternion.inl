#ifndef LINEARALGEBRA_QUATERNION_INL
#define LINEARALGEBRA_QUATERNION_INL

namespace linearalgebra {
    
    template<typename FPType>
    inline void Quaternion<FPType>::set(const FPType ix, const FPType iy, const FPType iz, const FPType iw){
        x = ix;
        y = iy;
        z = iz;
        w = iw;
    }
    
    template<typename FPType>
    inline void Quaternion<FPType>::set(const FPType iv[4]){
        x = iv[0];
        y = iv[1];
        z = iv[2];
        w = iv[3];
    }
    
    template<typename FPType>
    inline bool Quaternion<FPType>::normalize(void) {
        FPType l = x * x + y * y + z * z;
        if(l > kEPS) {
            l = 1.0 / sqrt(l);
            x *= l;
            y *= l;
            z *= l;
            return true;
        }
        return false;
    }
    
    template<typename FPType>
    inline Matrix4<FPType> Quaternion<FPType>::getMatrix(void) const {
        Matrix4<FPType> m;
        
        m.m00 = x * x - y * y - z * z + w * w;
        m.m01 = 2.0 * (x * y + z * w);
        m.m02 = 2.0 * (x * z - y * w);
        m.m03 = 0.0;
        
        m.m10 = 2.0 * (x * y - z * w);
        m.m11 = -x * x + y * y - z * z + w * w;
        m.m12 = 2.0 * (y * z + x * w);
        m.m13 = 0.0;
        
        m.m20 = 2.0 * (x * z + y * w);
        m.m21 = 2.0 * (y * z - x * w);
        m.m22 = -x * x - y * y + z * z + w * w;
        m.m23 = 0.0;
        
        m.m30 = 0.0;
        m.m31 = 0.0;
        m.m32 = 0.0;
        m.m33 = 1.0;
        
        return m;
    }
    
    template<typename FPType>
    inline bool Quaternion<FPType>::hasRotation(void) const {
        return (x == 0.0) && (y == 0.0) && (z == 0.0) && (w == 1.0);
    }
    
    template<typename FPType>
    inline Quaternion<FPType> Quaternion<FPType>::makeRotation(const FPType rad, const FPType ax, const FPType ay, const FPType az) {
        Quaternion<FPType> q;
        FPType s = sin(rad * 0.5);
        FPType c = cos(rad * 0.5);
        
        q.x = ax * s;
        q.y = ay * s;
        q.z = az * s;
        q.w = c;
        
        return q;
    }
}

#endif
