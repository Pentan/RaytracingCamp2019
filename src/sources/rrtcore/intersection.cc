
#include "intersection.h"
#include "sceneobject.h"

using namespace r1h;

Intersection::Intersection():
    distance(kINF),
    position(),
	hitNormal(),
    geometryNormal(),
    //tangent(0.0),
    objectId(kNoIntersected)
{}

Intersection::~Intersection() {
}

/*
Vector3 Intersection::orientingNormal(const Ray &ray) const {
	return (Vector3::dot(normal, ray.direction) < 0.0)? normal : (normal * -1.0);
}
*/
void Intersection::clear() {
    distance = kINF;
    objectId = -1;
}

/////////

FinalIntersection::FinalIntersection(const Intersection& isect) :
    Intersection(isect)
{
}

FinalIntersection::~FinalIntersection() {
}

void FinalIntersection::computeTangentSpaceWithShadingNormal(const Vector3& shdnorm) {
    // shading params
    shadingNormal = shdnorm;
    Vector4 geomtan = objectRef->getGeometry()->computeTangent(this);
    shadingTangent = Vector3(geomtan.x, geomtan.y, geomtan.z);
    shadingCotangent = Vector3::cross(shadingTangent, shadingNormal) * geomtan.w;
    shadingCotangent.normalize();
    
    // compute basis
    Vector3 tangent = Vector3::cross(Vector3(0.0, 0.0, 1.0), shadingNormal);
    if(!tangent.normalize()) {
        tangent.set(1.0, 0.0, 0.0);
    }
    Vector3 basisx = Vector3(tangent.x, tangent.y, tangent.z);
    Vector3 basisy;
    Vector3 basisz = shadingNormal;
    
    basisy = Vector3::cross(basisz, basisx);
    basisy.normalize();
    
    tangentSpaceBasis.setBasis(basisx, basisy, basisz);
    inverseTangentSpaceBasis = Matrix4::transposed(tangentSpaceBasis);
}

/*
Vector3 FinalIntersection::smallOffsetedPosition(bool into) const {
    if(into) {
        return position + shadingNormal * kSmallOffset;
    } else {
        return position - shadingNormal * kSmallOffset;
    }
}
*/

