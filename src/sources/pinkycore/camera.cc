#include "camera.h"

#include "pptypes.h"
using namespace PinkyPi;

Camera::Camera():
    focalLength(1.0),
    fNumber(0.0),
    focusDistance(1.0)
{
}

Camera::~Camera() {}

void Camera::initWithType(CameraType t)
{
    type = t;
    switch (type) {
        default:
        case kPerspectiveCamera:
            perspective.aspect = 1.0;
            perspective.yfov = 1.0;
            perspective.znear = 0.0;
            perspective.zfar = 1e8;
            break;
        case kOrthographicsCamera:
            orthographics.xmag = 1.0;
            orthographics.ymag = 1.0;
            orthographics.znear = 0.0;
            orthographics.zfar = 1e8;
            break;
    }
}

void Camera::setGlobalTransform(const Matrix4 &m) {
    globalTransform = m;
    invGlobalTransform = Matrix4::inverted(m, nullptr);
}
