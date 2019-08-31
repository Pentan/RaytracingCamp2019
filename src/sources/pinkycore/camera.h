#ifndef PINKYPI_CAMERA_H
#define PINKYPI_CAMERA_H

#include <string>
#include <vector>
#include <memory>
#include "pptypes.h"

namespace PinkyPi {
    
    class Camera {
    public:
        enum CameraType {
            kPerspectiveCamera,
            kOrthographicsCamera
        };
        
    public:
        Camera();
        ~Camera();
        
        void initWithType(CameraType t);
        
        //
        std::string name;
        CameraType type;
        
        union {
            struct {
                PPFloat aspect;
                PPFloat yfov;
                PPFloat zfar;
                PPFloat znear;
            } perspective;

            struct {
                PPFloat xmag;
                PPFloat ymag;
                PPFloat zfar;
                PPFloat znear;
            } orthographics;
        };
        
        PPFloat focalLength;
        PPFloat fNumber;
        PPFloat focusDistance;
    };
}

#endif
