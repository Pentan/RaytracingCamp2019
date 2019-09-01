﻿
#include "aabbgeometry.h"

using namespace r1h;

const int AABBGeometry::kTypeID = 'AABB';


AABBGeometry::AABBGeometry(const Vector3 min, const Vector3 max):
	Geometry(), aabb(min, max)
{}

AABBGeometry::AABBGeometry(const AABB inaabb):
	Geometry(), aabb(inaabb)
{}

AABB AABBGeometry::getAABB(const Matrix4& tm) const {
	Vector3 p[8] = {
		Vector3(aabb.min.x, aabb.min.y, aabb.min.z),
		Vector3(aabb.max.x, aabb.min.y, aabb.min.z),
		Vector3(aabb.min.x, aabb.max.y, aabb.min.z),
		Vector3(aabb.max.x, aabb.max.y, aabb.min.z),
		Vector3(aabb.min.x, aabb.min.y, aabb.max.z),
		Vector3(aabb.max.x, aabb.min.y, aabb.max.z),
		Vector3(aabb.min.x, aabb.max.y, aabb.max.z),
		Vector3(aabb.max.x, aabb.max.y, aabb.max.z)
	};
	AABB ret;
	for(int i = 0; i < 8; i++) {
		ret.expand(Matrix4::transformV3(tm, p[i]));
	}
	return ret;
}

bool AABBGeometry::isIntersect(const Ray &ray, Intersection *intersect) const {
	double t;
	int axis;
	
	if(aabb.isIntersect(ray, &t, &axis)) {
		intersect->distance = t;
		intersect->position = ray.origin + ray.direction * t;
		intersect->materialId = 0;
		
		double d = (ray.direction.v[axis] > 0.0)? -1.0 : 1.0;
		switch(axis) {
			case 0:
				intersect->hitNormal = Vector3(d, 0.0, 0.0);
				break;
			case 1:
				intersect->hitNormal = Vector3(0.0, d, 0.0);
				break;
			case 2:
				intersect->hitNormal = Vector3(0.0, 0.0, d);
				break;
		}
        intersect->geometryNormal = intersect->hitNormal;
        
		return true;
	}
	return false;
}

Geometry::SamplePoint AABBGeometry::getSamplePoint(Random *rng) const {
    Geometry::SamplePoint ret;
    
    Vector3 aabbsize = aabb.getSize();
    R1hFPType a0 = aabbsize.x * aabbsize.y;
    R1hFPType a1 = aabbsize.y * aabbsize.z + a0;
    R1hFPType at = aabbsize.z * aabbsize.x + a1; // area total
    
    R1hFPType ar = rng->nextf() * at * 2.0;
    
    bool ismin = true;
    if(ar >= at) {
        ar -= at;
        ismin = false;
    }
    
    Vector3 p;
    if(ar < a0) { // xy
        ret.position.x = aabb.min.x + rng->nextf() * aabbsize.x;
        ret.position.y = aabb.min.y + rng->nextf() * aabbsize.y;
        ret.position.z = ismin? aabb.min.z : aabb.max.z;
        ret.normal.set(0.0, 0.0, (ismin? -1.0 : 1.0));
        
    } else if(ar < a0) { // yz
        ret.position.x = (ismin)? aabb.min.x : aabb.max.x;
        ret.position.y = aabb.min.y + rng->nextf() * aabbsize.y;
        ret.position.z = aabb.min.z + rng->nextf() * aabbsize.z;
        ret.normal.set((ismin? -1.0 : 1.0), 0.0, 0.0);
    } else { // zx
        ret.position.x = aabb.min.x + rng->nextf() * aabbsize.x;
        ret.position.y = (ismin)? aabb.min.y : aabb.max.y;
        ret.position.z = aabb.min.z + rng->nextf() * aabbsize.z;
        ret.normal.set(0.0, (ismin? -1.0 : 1.0), 0.0);
    }
    ret.pdf = 1.0 / at;
    
    return ret;
}
