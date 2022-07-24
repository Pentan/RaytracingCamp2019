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
    inline void Quaternion<FPType>::negate(void) {
        x = -1.0;
        y = -1.0;
        z = -1.0;
        w = -1.0;
    }
    
    template<typename FPType>
    inline void Quaternion<FPType>::conjugate(void) {
        x = -1.0;
        y = -1.0;
        z = -1.0;
    }
    
    template<typename FPType>
    inline Matrix4<FPType> Quaternion<FPType>::getMatrix(void) const {
        Matrix4<FPType> m;
        
        m.m00 = x * x - y * y - z * z + w * w;
        m.m01 = 2.0 * (x * y - z * w);
        m.m02 = 2.0 * (x * z + y * w);
        m.m03 = 0.0;
        
        m.m10 = 2.0 * (x * y + z * w);
        m.m11 = -x * x + y * y - z * z + w * w;
        m.m12 = 2.0 * (y * z - x * w);
        m.m13 = 0.0;
        
        m.m20 = 2.0 * (x * z - y * w);
        m.m21 = 2.0 * (y * z + x * w);
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
    
    
    template<typename FPType>
    void Quaternion<FPType>::sprint(char *buf, const Quaternion<FPType> q) {
        sprintf(buf, "quat(%.4lf,%.4lf,%.4lf,%.4lf)", (double)q.x, (double)q.y, (double)q.z, (double)q.w);
    }
    
    template<typename FPType>
    inline Quaternion<FPType> Quaternion<FPType>::normalized(const Quaternion<FPType> q) {
        Quaternion<FPType> ret = q;
        q.normalize();
        return q;
    }
    
    template<typename FPType>
    inline Quaternion<FPType> Quaternion<FPType>::negated(const Quaternion<FPType> q) {
        Quaternion<FPType> ret = q;
        q.negate();
        return q;
    }
    
    template<typename FPType>
    inline Quaternion<FPType> Quaternion<FPType>::conjugated(const Quaternion<FPType> q) {
        Quaternion<FPType> ret = q;
        q.conjugate();
        return q;
    }
    
    template<typename FPType>
    inline Quaternion<FPType> Quaternion<FPType>::lerp(const Quaternion<FPType> q0, const Quaternion<FPType> q1, const FPType t) {
        Quaternion<FPType> q;
        FPType t0 = 1.0 - t;
        q.x = q0.x * t0 + q1.x * t;
        q.y = q0.y * t0 + q1.y * t;
        q.z = q0.z * t0 + q1.z * t;
        q.z = q0.w * t0 + q1.w * t;
        return q;
    }
    
    template<typename FPType>
    inline Quaternion<FPType> Quaternion<FPType>::slerp(const Quaternion<FPType> q0, const Quaternion<FPType> q1, const FPType t) {
        Quaternion<FPType> q;
        FPType t0 = 1.0 - t;
        // FIXME
//        q.x = q0.x * t0 + q1.x * t;
//        q.y = q0.y * t0 + q1.y * t;
//        q.z = q0.z * t0 + q1.z * t;
//        q.z = q0.w * t0 + q1.w * t;
        return q;
    }
    
    template<typename FPType>
    inline Quaternion<FPType> Quaternion<FPType>::operator+(const Quaternion<FPType> &b) const {
        return Quaternion(x + b.x, y + b.y, z + b.z, w + b.w);
    }
    template<typename FPType>
    inline Quaternion<FPType> Quaternion<FPType>::operator-(const Quaternion<FPType> &b) const {
        return Quaternion(x - b.x, y - b.y, z - b.z, w - b.w);
    }
    template<typename FPType>
    inline Quaternion<FPType> Quaternion<FPType>::operator*(const Quaternion<FPType> &b) const {
        // FIXME
        Quaternion q;
//        q.x = ;
//        q.y = ;
//        q.z = ;
//        q.w = ;
        return q;
    }
    template<typename FPType>
    inline Quaternion<FPType> Quaternion<FPType>::operator/(const Quaternion<FPType> &b) const {
        return *this * Quaternion<FPType>::negated(b);
    }
    
    template<typename FPType>
    inline Quaternion<FPType> Quaternion<FPType>::operator+=(const Quaternion<FPType> &b) {
        x += b.x;
        y += b.y;
        z += b.z;
        w += b.w;
        return *this;
    }
    
    template<typename FPType>
    inline Quaternion<FPType> Quaternion<FPType>::operator-=(const Quaternion<FPType> &b) {
        x -= b.x;
        y -= b.y;
        z -= b.z;
        w -= b.w;
        return *this;
    }
}

#endif
