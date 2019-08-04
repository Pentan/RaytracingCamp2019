#ifndef PINKYPI_TYPES_H
#define PINKYPI_TYPES_H

#include <linearalgebra/linearalgebra.h>

namespace PinkyPi {

    // math
    typedef double PPFPType;

    const PPFPType kPI = 3.14159265358979323846;
    const PPFPType kINF = 1e128;
    const PPFPType kEPS = 1e-8;
    #define kSmallOffset    kEPS

    typedef linearalgebra::Vector3<PPFPType> Vector3;
    typedef linearalgebra::Vector4<PPFPType> Vector4;
    typedef linearalgebra::Matrix4<PPFPType> Matrix4;

    // color
    typedef double PPColorType;
    typedef linearalgebra::Vector3<PPColorType> Color;

}

#endif
