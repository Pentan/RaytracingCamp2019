#ifndef PINKYPI_TYPES_H
#define PINKYPI_TYPES_H

#include <linearalgebra/linearalgebra.h>

namespace PinkyPi {

    // math
    typedef double PPFloat;

    const PPFloat kPI = 3.14159265358979323846;
    const PPFloat kINF = 1e128;
    const PPFloat kEPS = 1e-8;
    #define kSmallOffset    kEPS

    typedef linearalgebra::Vector3<PPFloat> Vector3;
    typedef linearalgebra::Vector4<PPFloat> Vector4;
    typedef linearalgebra::Matrix4<PPFloat> Matrix4;
    typedef linearalgebra::Quaternion<PPFloat> Quaterion;

    // color
    typedef double PPColorType;
    typedef linearalgebra::Vector3<PPColorType> Color;

}

#endif
