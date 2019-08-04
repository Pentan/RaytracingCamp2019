#include <cmath>
#include <catch2/catch.hpp>
#include "../testsupport.h"

#include <linearalgebra/vector3.h>

namespace {
    typedef linearalgebra::Vector3<double> Vector3;
}

TEST_CASE("Vector3 construct and initialize", "[vector3]") {
    SECTION("implicit constructor") {
        Vector3 v;
        REQUIRE( v.x == 0.0 );
        REQUIRE( v.y == 0.0 );
        REQUIRE( v.z == 0.0 );
    }

    SECTION("1 args constructor") {
        Vector3 v(5.0);
        REQUIRE( v.x == 5.0 );
        REQUIRE( v.y == 5.0 );
        REQUIRE( v.z == 5.0 );
    }

    SECTION("3 args constructor") {
        Vector3 v(1.0, 2.0, 3.0);
        REQUIRE( v.x == 1.0 );
        REQUIRE( v.y == 2.0 );
        REQUIRE( v.z == 3.0 );
    }

    SECTION("value acces") {
        Vector3 v(1.0, 2.0, 3.0);
        REQUIRE( v.x == 1.0 );
        REQUIRE( v.y == 2.0 );
        REQUIRE( v.z == 3.0 );

        REQUIRE( v.r == 1.0 );
        REQUIRE( v.g == 2.0 );
        REQUIRE( v.b == 3.0 );

        REQUIRE( v.s == 1.0 );
        REQUIRE( v.t == 2.0 );
        REQUIRE( v.p == 3.0 );

        REQUIRE( v.v[0] == 1.0 );
        REQUIRE( v.v[1] == 2.0 );
        REQUIRE( v.v[2] == 3.0 );
    }

    SECTION("set 3 args") {
        Vector3 v;
        v.set(4.0, 5.0, 6.0);
        REQUIRE( v.x == 4.0 );
        REQUIRE( v.y == 5.0 );
        REQUIRE( v.z == 6.0 );
    }
    
    SECTION("set array[3]") {
        Vector3 v;
        double a[3] = {10.0, 20.0, 30.0};
        v.set(a);
        REQUIRE( v.x == 10.0 );
        REQUIRE( v.y == 20.0 );
        REQUIRE( v.z == 30.0 );
    }

    SECTION("copy") {
        Vector3 v0(9.0, 4.0, 2.0);
        Vector3 v1;
        v1 = v0;
        REQUIRE( v1.x == v0.x );
        REQUIRE( v1.y == v0.y );
        REQUIRE( v1.z == v0.z );
    }
}

TEST_CASE("Vector3 length", "[vector3]") {
    Vector3 v(2.0, 3.0, 6.0);
    REQUIRE( v.length() == Approx(7.0).margin(kTestEPS) );
}

TEST_CASE("Vector3 is zero", "[vector3]") {
    SECTION("perfect zero") {
        Vector3 v;
        v.set(0.0, 0.0, 0.0);
        REQUIRE( v.isZero() );
        v.set(kTestEPS, kTestEPS, kTestEPS);
        REQUIRE_FALSE( v.isZero() );
    }
    
    SECTION("near zero") {
        Vector3 v;
        v.set(1e-2, 1e-2, 1e-2);
        REQUIRE( v.isZero(1e-2 * 1.1) );
        REQUIRE_FALSE( v.isZero(1e-2) );
        REQUIRE_FALSE( v.isZero(1e-3) );
        
    }
}

TEST_CASE("Vector3 normalize", "[vector3]") {
    Vector3 v(4.0, 5.0, 6.0);
    REQUIRE( v.normalize() );
    REQUIRE( v.length() == Approx(1.0).margin(kTestEPS) );
    
    v.set(0.0, 0.0, 0.0);
    REQUIRE_FALSE( v.normalize() );
}

TEST_CASE("Vector3 negate", "[vector3]") {
    Vector3 v(1.0, -2.0, 3.0);
    v.negate();
    REQUIRE( v.x == -1.0 );
    REQUIRE( v.y == 2.0 );
    REQUIRE( v.z == -3.0 );
}

TEST_CASE("Vector3 max component", "[vector3]") {
    Vector3 v;
    int i;
    
    v.set(5.0, 2.0, -8.0);
    REQUIRE( v.getMaxComponent() == 5.0 );
    REQUIRE( v.getMaxComponent(&i) == 5.0 );
    REQUIRE( i == linearalgebra::kX );
    
    v.set(-9.0, 7.0, 3.0);
    REQUIRE( v.getMaxComponent() == 7.0 );
    REQUIRE( v.getMaxComponent(&i) == 7.0 );
    REQUIRE( i == linearalgebra::kY );
    
    v.set(-3.0, -2.0, -1.0);
    REQUIRE( v.getMaxComponent() == -1.0 );
    REQUIRE( v.getMaxComponent(&i) == -1.0 );
    REQUIRE( i == linearalgebra::kZ );
}

TEST_CASE("Vector3 min component", "[vector3]") {
    Vector3 v;
    int i;
    
    v.set(5.0, 6.0, 7.0);
    REQUIRE( v.getMinComponent() == 5.0 );
    REQUIRE( v.getMinComponent(&i) == 5.0 );
    REQUIRE( i == linearalgebra::kX );
    
    v.set(10.0, 7.0, 9.0);
    REQUIRE( v.getMinComponent() == 7.0 );
    REQUIRE( v.getMinComponent(&i) == 7.0 );
    REQUIRE( i == linearalgebra::kY );
    
    v.set(1.0, -0.5, -1.0);
    REQUIRE( v.getMinComponent() == -1.0 );
    REQUIRE( v.getMinComponent(&i) == -1.0 );
    REQUIRE( i == linearalgebra::kZ );
}

TEST_CASE("Vector3 distance", "[vector3]") {
    Vector3 v0(1.0, 2.0, 3.0);
    Vector3 v1(3.0, 5.0, -3.0);
    REQUIRE( Vector3::distance(v0, v1) == Approx(7.0).margin(kTestEPS) );
}

TEST_CASE("Vector3 normalized", "[vector3]") {
    Vector3 v0(1.0, 2.0, 3.0);
    Vector3 v = Vector3::normalized(v0);
    
    REQUIRE( v.length() == Approx(1.0).margin(kTestEPS) );
    
    v0.normalize();
    REQUIRE( v.x == v0.x );
    REQUIRE( v.y == v0.y );
    REQUIRE( v.z == v0.z );
}

TEST_CASE("Vector3 negated", "[vector3]") {
    Vector3 v0(1.0, 2.0, 3.0);
    Vector3 v = Vector3::negated(v0);
    
    v0.negate();
    REQUIRE( v.x == v0.x );
    REQUIRE( v.y == v0.y );
    REQUIRE( v.z == v0.z );
}

TEST_CASE("Vector3 2 vector operation", "[vector3]") {
    Vector3 v0(3.0, 4.0, -9.0);
    Vector3 v1(1.0, 2.0, 3.0);
    
    SECTION("multiply each component") {
        Vector3 v = Vector3::mul(v0, v1);
        REQUIRE( v.x == 3.0 );
        REQUIRE( v.y == 8.0 );
        REQUIRE( v.z == -27.0 );
    }
    
    SECTION("divide each component") {
        Vector3 v = Vector3::div(v0, v1);
        REQUIRE( v.x == 3.0 );
        REQUIRE( v.y == 2.0 );
        REQUIRE( v.z == -3.0 );
    }
}

TEST_CASE("Vector3 dot product", "[vector3]") {
    Vector3 v0(3.0, 4.0, -9.0);
    Vector3 v1(1.0, 2.0, 3.0);
    REQUIRE( Vector3::dot(v0, v1) == Approx(-16.0).margin(kTestEPS) );
}

TEST_CASE("Vector3 cross product", "[vector3]") {
    Vector3 v = Vector3::cross(Vector3(1.0, 1.0, 1.0), Vector3(1.0, 1.0, -1.0));
    REQUIRE( v.x == Approx(-2.0).margin(kTestEPS) );
    REQUIRE( v.y == Approx(2.0).margin(kTestEPS) );
    REQUIRE( v.z == Approx(0.0).margin(kTestEPS) );
}

TEST_CASE("Vector3 lerp", "[vector3]") {
    Vector3 v = Vector3::lerp(Vector3(1.0, 2.0, 3.0), Vector3(3.0, 4.0, 5.0), 0.5);
    REQUIRE( v.x == Approx(2.0).margin(kTestEPS) );
    REQUIRE( v.y == Approx(3.0).margin(kTestEPS) );
    REQUIRE( v.z == Approx(4.0).margin(kTestEPS) );
}

TEST_CASE("Vector3 project", "[vector3]") {
    Vector3 v = Vector3::project(Vector3(1.0, 2.0, 3.0), Vector3(0.0, 3.0, 0.0));
    REQUIRE( v.x == Approx(0.0).margin(kTestEPS) );
    REQUIRE( v.y == Approx(2.0).margin(kTestEPS) );
    REQUIRE( v.z == Approx(0.0).margin(kTestEPS) );
}

TEST_CASE("Vector3 operators", "[vector3]") {
    Vector3 v0(1.0, 2.0, 3.0);
    Vector3 v1(-4.0, 5.0, 6.0);
    
    SECTION("addition") {
        Vector3 v = v0 + v1;
        REQUIRE( v.x == -3.0 );
        REQUIRE( v.y == 7.0 );
        REQUIRE( v.z == 9.0 );
    }
    
    SECTION("subtruct") {
        Vector3 v = v0 - v1;
        REQUIRE( v.x == 5.0 );
        REQUIRE( v.y == -3.0 );
        REQUIRE( v.z == -3.0 );
    }
    
    SECTION("multuply scalar") {
        Vector3 v = v0 * 2.0;
        REQUIRE( v.x == 2.0 );
        REQUIRE( v.y == 4.0 );
        REQUIRE( v.z == 6.0 );
    }
    
    SECTION("divide by scalar") {
        Vector3 v = v0 / 2.0;
        REQUIRE( v.x == 0.5 );
        REQUIRE( v.y == 1.0 );
        REQUIRE( v.z == 1.5 );
    }
    
    SECTION("addition and substitution") {
        Vector3 v = v0;
        v += v1;
        REQUIRE( v.x == -3.0 );
        REQUIRE( v.y == 7.0 );
        REQUIRE( v.z == 9.0 );
    }
    
    SECTION("subtruct and substitution") {
        Vector3 v = v0;
        v -= v1;
        REQUIRE( v.x == 5.0 );
        REQUIRE( v.y == -3.0 );
        REQUIRE( v.z == -3.0 );
    }
}

//TEST_CASE("Vector3 ", "[vector3]") {
//    Vector3 v();
//    REQUIRE(  );
//}

