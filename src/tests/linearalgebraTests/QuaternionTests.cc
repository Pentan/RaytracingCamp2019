#include <cmath>
#include <doctest.h>
#include "../testsupport.h"

#include <linearalgebra/quaternion.h>

namespace {
    typedef linearalgebra::Vector3<double> Vector3;
    typedef linearalgebra::Matrix4<double> Matrix4;
    typedef linearalgebra::Quaternion<double> Quaternion;
}

TEST_CASE("Quaternion construct and initialize [quaternion]") {
    SUBCASE("implicit constructor") {
        Quaternion q;
        REQUIRE( q.x == 0.0 );
        REQUIRE( q.y == 0.0 );
        REQUIRE( q.z == 0.0 );
        REQUIRE( q.w == 1.0 );
    }
    
    SUBCASE("args constructor") {
        Quaternion q(1.0, 2.0, 3.0, 4.0);
        REQUIRE( q.x == 1.0 );
        REQUIRE( q.y == 2.0 );
        REQUIRE( q.z == 3.0 );
        REQUIRE( q.w == 4.0 );
    }
    
    SUBCASE("set values") {
        Quaternion q;
        q.set(1.0, 2.0, 3.0, 4.0);
        REQUIRE( q.x == 1.0 );
        REQUIRE( q.y == 2.0 );
        REQUIRE( q.z == 3.0 );
        REQUIRE( q.w == 4.0 );
    }
    
    SUBCASE("set array values") {
        Quaternion q;
        double v[4] = {1.0, 2.0, 3.0, 4.0};
        q.set(v);
        REQUIRE( q.x == 1.0 );
        REQUIRE( q.y == 2.0 );
        REQUIRE( q.z == 3.0 );
        REQUIRE( q.w == 4.0 );
    }
}
