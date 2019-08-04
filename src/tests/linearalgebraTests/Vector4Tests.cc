
#include <cmath>
#include <catch2/catch.hpp>
#include "../testsupport.h"

#include <linearalgebra/vector3.h>
#include <linearalgebra/vector4.h>

namespace {
    typedef linearalgebra::Vector3<double> Vector3;
    typedef linearalgebra::Vector4<double> Vector4;
}

TEST_CASE("Vector4 construct and initialize", "[vector4]") {
    SECTION("implicit constructor") {
        Vector4 v;
        REQUIRE( v.x == 0.0 );
        REQUIRE( v.y == 0.0 );
        REQUIRE( v.z == 0.0 );
        REQUIRE( v.w == 0.0 );
    }
    
    SECTION("1 args constructor") {
        Vector4 v(5.0);
        REQUIRE( v.x == 5.0 );
        REQUIRE( v.y == 5.0 );
        REQUIRE( v.z == 5.0 );
        REQUIRE( v.w == 5.0 );
    }
    
    SECTION("3 args constructor") {
        Vector4 v(1.0, 2.0, 3.0, 4.0);
        REQUIRE( v.x == 1.0 );
        REQUIRE( v.y == 2.0 );
        REQUIRE( v.z == 3.0 );
        REQUIRE( v.w == 4.0 );
    }
    
    SECTION("value acces") {
        Vector4 v(1.0, 2.0, 3.0, 4.0);
        REQUIRE( v.x == 1.0 );
        REQUIRE( v.y == 2.0 );
        REQUIRE( v.z == 3.0 );
        REQUIRE( v.w == 4.0 );
        
        REQUIRE( v.r == 1.0 );
        REQUIRE( v.g == 2.0 );
        REQUIRE( v.b == 3.0 );
        REQUIRE( v.a == 4.0 );
        
        REQUIRE( v.s == 1.0 );
        REQUIRE( v.t == 2.0 );
        REQUIRE( v.p == 3.0 );
        REQUIRE( v.q == 4.0 );
        
        REQUIRE( v.v[0] == 1.0 );
        REQUIRE( v.v[1] == 2.0 );
        REQUIRE( v.v[2] == 3.0 );
        REQUIRE( v.v[3] == 4.0 );
    }
    
    SECTION("set 3 args") {
        Vector4 v;
        v.set(4.0, 5.0, 6.0, 7.0);
        REQUIRE( v.x == 4.0 );
        REQUIRE( v.y == 5.0 );
        REQUIRE( v.z == 6.0 );
        REQUIRE( v.w == 7.0 );
    }
    
    SECTION("set array[4]") {
        Vector4 v;
        double a[4] = {10.0, 20.0, 30.0, 40.0};
        v.set(a);
        REQUIRE( v.x == 10.0 );
        REQUIRE( v.y == 20.0 );
        REQUIRE( v.z == 30.0 );
        REQUIRE( v.w == 40.0 );
    }
    
    SECTION("set Vector3 and scalar") {
        Vector4 v;
        Vector3 v3(1.0, 2.0, 3.0);
        v.set(v3, 4.0);
        REQUIRE( v.x == 1.0 );
        REQUIRE( v.y == 2.0 );
        REQUIRE( v.z == 3.0 );
        REQUIRE( v.w == 4.0 );
    }
    
    SECTION("copy") {
        Vector4 v0(9.0, 4.0, 2.0, 1.0);
        Vector4 v1;
        v1 = v0;
        REQUIRE( v1.x == v0.x );
        REQUIRE( v1.y == v0.y );
        REQUIRE( v1.z == v0.z );
        REQUIRE( v1.w == v0.w );
    }
}

TEST_CASE("Vector4 get xyz", "[vector4]") {
    Vector4 v(9.0, 3.0, 3.0, 1.0);
    Vector3 xyz = v.getXYZ();
    REQUIRE( xyz.x == v.x );
    REQUIRE( xyz.y == v.y );
    REQUIRE( xyz.z == v.z );
}

TEST_CASE("Vector4 length", "[vector4]") {
    Vector4 v(9.0, 3.0, 3.0, 1.0);
    REQUIRE( v.length() == Approx(10.0).margin(kTestEPS) );
}

TEST_CASE("Vector4 is zero", "[vector4]") {
    SECTION("perfect zero") {
        Vector4 v;
        v.set(0.0, 0.0, 0.0, 0.0);
        REQUIRE( v.isZero() );
        v.set(kTestEPS, kTestEPS, kTestEPS, kTestEPS);
        REQUIRE_FALSE( v.isZero() );
    }
    
    SECTION("near zero") {
        Vector4 v;
        v.set(1e-2, 1e-2, 1e-2, 1e-2);
        REQUIRE( v.isZero(1e-2 * 1.1) );
        REQUIRE_FALSE( v.isZero(1e-2) );
        REQUIRE_FALSE( v.isZero(1e-3) );
    }
}

TEST_CASE("Vector4 normalize", "[vector4]") {
    Vector4 v(4.0, 5.0, 6.0, 7.0);
    REQUIRE( v.normalize() );
    REQUIRE( v.length() == Approx(1.0).margin(kTestEPS) );
    
    v.set(0.0, 0.0, 0.0, 0.0);
    REQUIRE_FALSE( v.normalize() );
}

TEST_CASE("Vector4 negate", "[vector4]") {
    Vector4 v(1.0, -2.0, 3.0, -4.0);
    v.negate();
    REQUIRE( v.x == -1.0 );
    REQUIRE( v.y == 2.0 );
    REQUIRE( v.z == -3.0 );
    REQUIRE( v.w == 4.0 );
}

TEST_CASE("Vector4 max component", "[vector4]") {
    Vector4 v;
    int i;
    
    v.set(5.0, 2.0, -8.0, -4.0);
    REQUIRE( v.getMaxComponent() == 5.0 );
    REQUIRE( v.getMaxComponent(&i) == 5.0 );
    REQUIRE( i == linearalgebra::kX );
    
    v.set(-9.0, 7.0, 3.0, -1.0);
    REQUIRE( v.getMaxComponent() == 7.0 );
    REQUIRE( v.getMaxComponent(&i) == 7.0 );
    REQUIRE( i == linearalgebra::kY );
    
    v.set(-3.0, -2.0, -1.0, -6.0);
    REQUIRE( v.getMaxComponent() == -1.0 );
    REQUIRE( v.getMaxComponent(&i) == -1.0 );
    REQUIRE( i == linearalgebra::kZ );
    
    v.set(3.0, -2.0, -1.0, 6.0);
    REQUIRE( v.getMaxComponent() == 6.0 );
    REQUIRE( v.getMaxComponent(&i) == 6.0 );
    REQUIRE( i == linearalgebra::kW );
}

TEST_CASE("Vector4 min component", "[vector4]") {
    Vector4 v;
    int i;
    
    v.set(5.0, 6.0, 7.0, 8.0);
    REQUIRE( v.getMinComponent() == 5.0 );
    REQUIRE( v.getMinComponent(&i) == 5.0 );
    REQUIRE( i == linearalgebra::kX );
    
    v.set(10.0, 7.0, 9.0, 12.0);
    REQUIRE( v.getMinComponent() == 7.0 );
    REQUIRE( v.getMinComponent(&i) == 7.0 );
    REQUIRE( i == linearalgebra::kY );
    
    v.set(1.0, -0.5, -1.0, 2.0);
    REQUIRE( v.getMinComponent() == -1.0 );
    REQUIRE( v.getMinComponent(&i) == -1.0 );
    REQUIRE( i == linearalgebra::kZ );
    
    v.set(1.0, -0.5, -1.0, -2.0);
    REQUIRE( v.getMinComponent() == -2.0 );
    REQUIRE( v.getMinComponent(&i) == -2.0 );
    REQUIRE( i == linearalgebra::kW );
}

TEST_CASE("Vector4 distance", "[vector4]") {
    Vector4 v0(1.0, 2.0, 3.0, 4.0);
    Vector4 v1(10.0, 5.0, 0.0, 3.0);
    REQUIRE( Vector4::distance(v0, v1) == Approx(10.0).margin(kTestEPS) );
}

TEST_CASE("Vector4 normalized", "[vector4]") {
    Vector4 v0(1.0, 2.0, 3.0, 4.0);
    Vector4 v = Vector4::normalized(v0);
    
    REQUIRE( v.length() == Approx(1.0).margin(kTestEPS) );
    
    v0.normalize();
    REQUIRE( v.x == v0.x );
    REQUIRE( v.y == v0.y );
    REQUIRE( v.z == v0.z );
}

TEST_CASE("Vector4 negated", "[vector4]") {
    Vector4 v0(1.0, 2.0, 3.0, 4.0);
    Vector4 v = Vector4::negated(v0);
    
    v0.negate();
    REQUIRE( v.x == v0.x );
    REQUIRE( v.y == v0.y );
    REQUIRE( v.z == v0.z );
    REQUIRE( v.w == v0.w );
}

TEST_CASE("Vector4 2 vector operation", "[vector4]") {
    Vector4 v0(3.0, 4.0, -9.0, 12.0);
    Vector4 v1(1.0, 2.0, 3.0, 4.0);
    
    SECTION("multiply each component") {
        Vector4 v = Vector4::mul(v0, v1);
        REQUIRE( v.x == 3.0 );
        REQUIRE( v.y == 8.0 );
        REQUIRE( v.z == -27.0 );
        REQUIRE( v.w == 48.0 );
    }
    
    SECTION("divide each component") {
        Vector4 v = Vector4::div(v0, v1);
        REQUIRE( v.x == 3.0 );
        REQUIRE( v.y == 2.0 );
        REQUIRE( v.z == -3.0 );
        REQUIRE( v.w == 3.0 );
    }
}

TEST_CASE("Vector4 dot product", "[vector4]") {
    Vector4 v0(3.0, 4.0, -9.0, 1.0);
    Vector4 v1(1.0, 2.0, 3.0, 4.0);
    REQUIRE( Vector4::dot(v0, v1) == Approx(-12.0).margin(kTestEPS) );
}

TEST_CASE("Vector4 lerp", "[vector4]") {
    Vector4 v = Vector4::lerp(Vector4(1.0, 2.0, 3.0, 4.0), Vector4(3.0, 4.0, 5.0, 2.0), 0.5);
    REQUIRE( v.x == Approx(2.0).margin(kTestEPS) );
    REQUIRE( v.y == Approx(3.0).margin(kTestEPS) );
    REQUIRE( v.z == Approx(4.0).margin(kTestEPS) );
    REQUIRE( v.w == Approx(3.0).margin(kTestEPS) );
}

TEST_CASE("Vector4 project", "[vector4]") {
    Vector4 v = Vector4::project(Vector4(1.0, 2.0, 3.0, 4.0), Vector4(0.0, 3.0, 0.0, 0.0));
    REQUIRE( v.x == Approx(0.0).margin(kTestEPS) );
    REQUIRE( v.y == Approx(2.0).margin(kTestEPS) );
    REQUIRE( v.z == Approx(0.0).margin(kTestEPS) );
    REQUIRE( v.w == Approx(0.0).margin(kTestEPS) );
}

TEST_CASE("Vector4 operators", "[vector4]") {
    Vector4 v0(1.0, 2.0, 3.0, 4.0);
    Vector4 v1(-4.0, 5.0, 6.0, -7.0);
    
    SECTION("addition") {
        Vector4 v = v0 + v1;
        REQUIRE( v.x == -3.0 );
        REQUIRE( v.y == 7.0 );
        REQUIRE( v.z == 9.0 );
        REQUIRE( v.w == -3.0 );
    }
    
    SECTION("subtruct") {
        Vector4 v = v0 - v1;
        REQUIRE( v.x == 5.0 );
        REQUIRE( v.y == -3.0 );
        REQUIRE( v.z == -3.0 );
        REQUIRE( v.w == 11.0 );
    }
    
    SECTION("multuply scalar") {
        Vector4 v = v0 * 2.0;
        REQUIRE( v.x == 2.0 );
        REQUIRE( v.y == 4.0 );
        REQUIRE( v.z == 6.0 );
        REQUIRE( v.w == 8.0 );
    }
    
    SECTION("divide by scalar") {
        Vector4 v = v0 / 2.0;
        REQUIRE( v.x == 0.5 );
        REQUIRE( v.y == 1.0 );
        REQUIRE( v.z == 1.5 );
        REQUIRE( v.w == 2.0 );
    }
    
    SECTION("addition and substitution") {
        Vector4 v = v0;
        v += v1;
        REQUIRE( v.x == -3.0 );
        REQUIRE( v.y == 7.0 );
        REQUIRE( v.z == 9.0 );
        REQUIRE( v.w == -3.0 );
    }
    
    SECTION("subtruct and substitution") {
        Vector4 v = v0;
        v -= v1;
        REQUIRE( v.x == 5.0 );
        REQUIRE( v.y == -3.0 );
        REQUIRE( v.z == -3.0 );
        REQUIRE( v.w == 11.0 );
    }
}

//TEST_CASE("Vector4 ", "[vector4]") {
//    Vector4 v();
//    REQUIRE(  );
//}

