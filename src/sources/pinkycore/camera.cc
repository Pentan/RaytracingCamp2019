//
//  camera.cpp
//  PinkyCore
//
//  Created by SatoruNAKAJIMA on 2019/08/16.
//

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
            getRayFunc = (fNumber > 0.0) ? getThinLensRay : getPerspectiveRay;
            break;
        case kOrthographicsCamera:
            orthographics.xmag = 1.0;
            orthographics.ymag = 1.0;
            orthographics.znear = 0.0;
            orthographics.zfar = 1e8;
            getRayFunc = getOrthoRay;
            break;
    }
}

Ray Camera::getRay(PPFloat tx, PPFloat ty, Random* rng)
{
    return getRayFunc(this, tx, ty, rng);
}

Ray Camera::getThinLensRay(Camera* cam, PPFloat tx, PPFloat ty, Random* rng) {
    // FIXME
    PPFloat y = std::tan(cam->perspective.yfov * 0.5);
    Vector3 o(0.0, 0.0, 0.0);
    Vector3 d(tx * y * cam->perspective.aspect, ty * y, -1);
    d.normalize();
    return Ray(o, d);
}

Ray Camera::getPerspectiveRay(Camera* cam, PPFloat tx, PPFloat ty, Random* rng) {
    PPFloat y = std::tan(cam->perspective.yfov * 0.5);
    Vector3 o(0.0, 0.0, 0.0);
    Vector3 d(tx * y * cam->perspective.aspect, ty * y, -1);
    d.normalize();
    return Ray(o, d);
}

Ray Camera::getOrthoRay(Camera* cam, PPFloat tx, PPFloat ty, Random* rng) {
    Vector3 o(tx * cam->orthographics.xmag, ty * cam->orthographics.ymag, 0.0);
    Vector3 d(0.0, 0.0, -1.0);
    return Ray(o, d);
}
