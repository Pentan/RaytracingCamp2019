#include <catch2/catch.hpp>
#include "../testsupport.h"

#include <pinkycore/pptypes.h>
#include <pinkycore/ray.h>
#include <pinkycore/aabb.h>

using namespace PinkyPi;

namespace {
    bool IsHitRayAndAABB(const AABB& aabb, Vector3 o, Vector3 d, PPFPType n=1e-2, PPFPType f=1e8) {
        return aabb.isIntersect(Ray(o, Vector3::normalized(d)), n, f);
    }
}

TEST_CASE("AABB basic test", "[AABB]") {
    SECTION("construct with 2 scalar") {
        AABB aabb(1.0, 2.0);
        
        REQUIRE( aabb.min.x == 1.0 );
        REQUIRE( aabb.min.y == 1.0 );
        REQUIRE( aabb.min.z == 1.0 );
        REQUIRE( aabb.max.x == 2.0 );
        REQUIRE( aabb.max.y == 2.0 );
        REQUIRE( aabb.max.z == 2.0 );
    }
    
    SECTION("construct with min and max") {
        AABB aabb(Vector3(-1.0, -2.0, -3.0), Vector3(1.0, 2.0, 3.0));
        
        REQUIRE( aabb.min.x == -1.0 );
        REQUIRE( aabb.min.y == -2.0 );
        REQUIRE( aabb.min.z == -3.0 );
        REQUIRE( aabb.max.x == 1.0 );
        REQUIRE( aabb.max.y == 2.0 );
        REQUIRE( aabb.max.z == 3.0 );
    }
    
    SECTION("expand") {
        AABB aabb;
        
        aabb.expand(Vector3(1.0, 2.0, -4.0));
        aabb.expand(Vector3(0.0, -1.0, 3.0));
        
        REQUIRE( aabb.min.x == 0.0 );
        REQUIRE( aabb.min.y == -1.0 );
        REQUIRE( aabb.min.z == -4.0 );
        REQUIRE( aabb.max.x == 1.0 );
        REQUIRE( aabb.max.y == 2.0 );
        REQUIRE( aabb.max.z == 3.0 );
    }
}

TEST_CASE("AABB intersect") {
    AABB aabb(Vector3(-1.0), Vector3(1.0));
    
    REQUIRE( IsHitRayAndAABB(aabb, Vector3(0.0, 0.0, 5.0), Vector3(0.0, 0.0, -1.0)) );
    REQUIRE( IsHitRayAndAABB(aabb, Vector3(0.0, 0.0, 0.5), Vector3(0.0, 0.0, -1.0)) );
    REQUIRE( IsHitRayAndAABB(aabb, Vector3(0.0, 0.0, 10.0), Vector3(1.0, 1.0, 1.0-10.0)) );
    
    REQUIRE_FALSE( IsHitRayAndAABB(aabb, Vector3(0.0, 0.0, 5.0), Vector3(0.0, 0.0, 1.0)) );
    REQUIRE_FALSE( IsHitRayAndAABB(aabb, Vector3(0.0, 0.0, 5.0), Vector3(0.0, 0.5, 0.5)) );
    
    REQUIRE_FALSE( IsHitRayAndAABB(aabb, Vector3(0.0, 0.0, 5.0), Vector3(0.0, 0.0, -1.0), 10.0, 20.0) );
    REQUIRE_FALSE( IsHitRayAndAABB(aabb, Vector3(0.0, 0.0, 5.0), Vector3(0.0, 0.0, -1.0), 1e-2, 3.0) );
}
