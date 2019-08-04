#include <catch2/catch.hpp>
#include "../testsupport.h"

#include <linearalgebra/vector3.h>
#include <linearalgebra/vector4.h>
#include <linearalgebra/matrix4.h>

namespace {
    typedef linearalgebra::Vector3<double> Vector3;
    typedef linearalgebra::Vector4<double> Vector4;
    typedef linearalgebra::Matrix4<double> Matrix4;
    
#define Matrix4RequireIdentity(m) {\
    REQUIRE( m.m00 == 1.0 );\
    REQUIRE( m.m01 == 0.0 );\
    REQUIRE( m.m02 == 0.0 );\
    REQUIRE( m.m03 == 0.0 );\
    REQUIRE( m.m10 == 0.0 );\
    REQUIRE( m.m11 == 1.0 );\
    REQUIRE( m.m12 == 0.0 );\
    REQUIRE( m.m13 == 0.0 );\
    REQUIRE( m.m20 == 0.0 );\
    REQUIRE( m.m21 == 0.0 );\
    REQUIRE( m.m22 == 1.0 );\
    REQUIRE( m.m23 == 0.0 );\
    REQUIRE( m.m30 == 0.0 );\
    REQUIRE( m.m31 == 0.0 );\
    REQUIRE( m.m32 == 0.0 );\
    REQUIRE( m.m33 == 1.0 );\
    }
#define Matrix4RequireIdentityAprox(m, eps) {\
    REQUIRE( m.m00 == Approx(1.0).margin(eps) );\
    REQUIRE( m.m01 == Approx(0.0).margin(eps) );\
    REQUIRE( m.m02 == Approx(0.0).margin(eps) );\
    REQUIRE( m.m03 == Approx(0.0).margin(eps) );\
    REQUIRE( m.m10 == Approx(0.0).margin(eps) );\
    REQUIRE( m.m11 == Approx(1.0).margin(eps) );\
    REQUIRE( m.m12 == Approx(0.0).margin(eps) );\
    REQUIRE( m.m13 == Approx(0.0).margin(eps) );\
    REQUIRE( m.m20 == Approx(0.0).margin(eps) );\
    REQUIRE( m.m21 == Approx(0.0).margin(eps) );\
    REQUIRE( m.m22 == Approx(1.0).margin(eps) );\
    REQUIRE( m.m23 == Approx(0.0).margin(eps) );\
    REQUIRE( m.m30 == Approx(0.0).margin(eps) );\
    REQUIRE( m.m31 == Approx(0.0).margin(eps) );\
    REQUIRE( m.m32 == Approx(0.0).margin(eps) );\
    REQUIRE( m.m33 == Approx(1.0).margin(eps) );\
    }
#define Matrix4RequireEqual(m, a) {\
    REQUIRE( m.m00 == a[0] );\
    REQUIRE( m.m01 == a[1] );\
    REQUIRE( m.m02 == a[2] );\
    REQUIRE( m.m03 == a[3] );\
    REQUIRE( m.m10 == a[4] );\
    REQUIRE( m.m11 == a[5] );\
    REQUIRE( m.m12 == a[6] );\
    REQUIRE( m.m13 == a[7] );\
    REQUIRE( m.m20 == a[8] );\
    REQUIRE( m.m21 == a[9] );\
    REQUIRE( m.m22 == a[10] );\
    REQUIRE( m.m23 == a[11] );\
    REQUIRE( m.m30 == a[12] );\
    REQUIRE( m.m31 == a[13] );\
    REQUIRE( m.m32 == a[14] );\
    REQUIRE( m.m33 == a[15] );\
    }
#define Matrix4RequireEqualAprox(m, a, eps) {\
    REQUIRE( m.m00 == Approx(a[0]).margin(eps) );\
    REQUIRE( m.m01 == Approx(a[1]).margin(eps) );\
    REQUIRE( m.m02 == Approx(a[2]).margin(eps) );\
    REQUIRE( m.m03 == Approx(a[3]).margin(eps) );\
    REQUIRE( m.m10 == Approx(a[4]).margin(eps) );\
    REQUIRE( m.m11 == Approx(a[5]).margin(eps) );\
    REQUIRE( m.m12 == Approx(a[6]).margin(eps) );\
    REQUIRE( m.m13 == Approx(a[7]).margin(eps) );\
    REQUIRE( m.m20 == Approx(a[8]).margin(eps) );\
    REQUIRE( m.m21 == Approx(a[9]).margin(eps) );\
    REQUIRE( m.m22 == Approx(a[10]).margin(eps) );\
    REQUIRE( m.m23 == Approx(a[11]).margin(eps) );\
    REQUIRE( m.m30 == Approx(a[12]).margin(eps) );\
    REQUIRE( m.m31 == Approx(a[13]).margin(eps) );\
    REQUIRE( m.m32 == Approx(a[14]).margin(eps) );\
    REQUIRE( m.m33 == Approx(a[15]).margin(eps) );\
    }
}

TEST_CASE("Matrix4 construct and initialize", "[Matrix4]") {
    double a[16] = {
        1.0, 2.0, 3.0, 4.0,
        5.0, 6.0, 7.0, 8.0,
        9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    };
    
    SECTION("implicit constructor") {
        Matrix4 m;
        Matrix4RequireIdentity(m);
    }
    
    SECTION("construct with args") {
        Matrix4 m(
            1.0, 2.0, 3.0, 4.0,
            5.0, 6.0, 7.0, 8.0,
            9.0, 10.0, 11.0, 12.0,
            13.0, 14.0, 15.0, 16.0
        );
        REQUIRE( m.m00 == 1.0 );
        REQUIRE( m.m01 == 2.0 );
        REQUIRE( m.m02 == 3.0 );
        REQUIRE( m.m03 == 4.0 );
        REQUIRE( m.m10 == 5.0 );
        REQUIRE( m.m11 == 6.0 );
        REQUIRE( m.m12 == 7.0 );
        REQUIRE( m.m13 == 8.0 );
        REQUIRE( m.m20 == 9.0 );
        REQUIRE( m.m21 == 10.0 );
        REQUIRE( m.m22 == 11.0 );
        REQUIRE( m.m23 == 12.0 );
        REQUIRE( m.m30 == 13.0 );
        REQUIRE( m.m31 == 14.0 );
        REQUIRE( m.m32 == 15.0 );
        REQUIRE( m.m33 == 16.0 );
    }
    
    SECTION("construct with array") {
        Matrix4 m(a);
        Matrix4RequireEqual(m, a);
    }
    
    SECTION("set args") {
        Matrix4 m;
        Matrix4RequireIdentity(m);
        
        m.set(
          1.0, 2.0, 3.0, 4.0,
          5.0, 6.0, 7.0, 8.0,
          9.0, 10.0, 11.0, 12.0,
          13.0, 14.0, 15.0, 16.0
        );
        REQUIRE( m.m00 == 1.0 );
        REQUIRE( m.m01 == 2.0 );
        REQUIRE( m.m02 == 3.0 );
        REQUIRE( m.m03 == 4.0 );
        REQUIRE( m.m10 == 5.0 );
        REQUIRE( m.m11 == 6.0 );
        REQUIRE( m.m12 == 7.0 );
        REQUIRE( m.m13 == 8.0 );
        REQUIRE( m.m20 == 9.0 );
        REQUIRE( m.m21 == 10.0 );
        REQUIRE( m.m22 == 11.0 );
        REQUIRE( m.m23 == 12.0 );
        REQUIRE( m.m30 == 13.0 );
        REQUIRE( m.m31 == 14.0 );
        REQUIRE( m.m32 == 15.0 );
        REQUIRE( m.m33 == 16.0 );
    }
    
    SECTION("set array") {
        Matrix4 m;
        Matrix4RequireIdentity(m);
        
        m.set(a);
        Matrix4RequireEqual(m, a);
    }
    
    SECTION("set columns") {
        Vector4 c0(a[0], a[1], a[2], a[3]);
        Vector4 c1(a[4], a[5], a[6], a[7]);
        Vector4 c2(a[8], a[9], a[10], a[11]);
        Vector4 c3(a[12], a[13], a[14], a[15]);
        
        Matrix4 m;
        m.setColumns(c0, c1, c2, c3);
        Matrix4RequireEqual(m, a);
    }
    
    SECTION("set rows") {
        Vector4 r0(a[0], a[4], a[8], a[12]);
        Vector4 r1(a[1], a[5], a[9], a[13]);
        Vector4 r2(a[2], a[6], a[10], a[14]);
        Vector4 r3(a[3], a[7], a[11], a[15]);
        
        Matrix4 m;
        m.setRows(r0, r1, r2, r3);
        Matrix4RequireEqual(m, a);
    }
    
    SECTION("set identity") {
        Matrix4 m(a);
        m.setIdentity();
        Matrix4RequireIdentity(m);
    }
    
    SECTION("set translation") {
        Matrix4 m;
        m.setTranslation(1.0, 2.0, 3.0);
        
        double t[16] = {
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            1.0, 2.0, 3.0, 1.0
        };
        Matrix4RequireEqual(m, t);
    }
    
    SECTION("set rotation") {
        double angle = M_PI * 30.0 / 180.0;
        Matrix4 m;
        
        m.setRotation(angle, 1.0, 0.0, 0.0);
        double rx[16] = {
            1.0, 0.0, 0.0, 0.0,
            0.0,  cos(angle), sin(angle), 0.0,
            0.0, -sin(angle), cos(angle), 0.0,
            0.0, 0.0, 0.0, 1.0
        };
        Matrix4RequireEqualAprox(m, rx, kTestEPS);
        
        m.setRotation(angle, 0.0, 1.0, 0.0);
        double ry[16] = {
            cos(angle), 0.0, -sin(angle), 0.0,
                   0.0, 1.0,        0.0, 0.0,
            sin(angle), 0.0,  cos(angle), 0.0,
            0.0, 0.0, 0.0, 1.0
        };
        Matrix4RequireEqualAprox(m, ry, kTestEPS);
        
        m.setRotation(angle, 0.0, 0.0, 1.0);
        double rz[16] = {
             cos(angle), sin(angle), 0.0, 0.0,
            -sin(angle), cos(angle), 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
        Matrix4RequireEqualAprox(m, rz, kTestEPS);
    }
    
    SECTION("set scale") {
        Matrix4 m;
        m.setScale(2.0, 4.0, 6.0);
        double s[16] = {
            2.0, 0.0, 0.0, 0.0,
            0.0, 4.0, 0.0, 0.0,
            0.0, 0.0, 6.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
        Matrix4RequireEqual(m, s);
    }
    
    SECTION("set basis") {
        Vector3 vx(1.0, 2.0, 3.0);
        Vector3 vy(4.0, 5.0, 6.0);
        Vector3 vz(7.0, 8.0, 9.0);
        
        Matrix4 m;
        m.setBasis(vx, vy, vz);
        double b[16] = {
            vx.x, vy.x, vz.x, 0.0,
            vx.y, vy.y, vz.y, 0.0,
            vx.z, vy.z, vz.z, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
        Matrix4RequireEqual(m, b);
    }
}

TEST_CASE("Matrix4 column and row", "[Matrix4]") {
    Matrix4 m(
        1.0, 2.0, 3.0, 4.0,
        5.0, 6.0, 7.0, 8.0,
        9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    );
    
    SECTION("get column") {
        Vector4 c;
        c = m.getColumn(0);
        REQUIRE( c.x == 1.0 );
        REQUIRE( c.y == 2.0 );
        REQUIRE( c.z == 3.0 );
        REQUIRE( c.w == 4.0 );
        
        c = m.getColumn(1);
        REQUIRE( c.x == 5.0 );
        REQUIRE( c.y == 6.0 );
        REQUIRE( c.z == 7.0 );
        REQUIRE( c.w == 8.0 );
        
        c = m.getColumn(2);
        REQUIRE( c.x == 9.0 );
        REQUIRE( c.y == 10.0 );
        REQUIRE( c.z == 11.0 );
        REQUIRE( c.w == 12.0 );
        
        c = m.getColumn(3);
        REQUIRE( c.x == 13.0 );
        REQUIRE( c.y == 14.0 );
        REQUIRE( c.z == 15.0 );
        REQUIRE( c.w == 16.0 );
    }
    
    SECTION("set column") {
        Matrix4 n;
        n.setColumn(0, Vector4(1.0, 2.0, 3.0, 4.0));
        n.setColumn(1, Vector4(5.0, 6.0, 7.0, 8.0));
        n.setColumn(2, Vector4(9.0, 10.0, 11.0, 12.0));
        n.setColumn(3, Vector4(13.0, 14.0, 15.0, 16.0));
        double a[16] = {
            1.0, 2.0, 3.0, 4.0,
            5.0, 6.0, 7.0, 8.0,
            9.0, 10.0, 11.0, 12.0,
            13.0, 14.0, 15.0, 16.0
        };
        Matrix4RequireEqual(n, a);
    }
    
    SECTION("get row") {
        Vector4 c;
        c = m.getRow(0);
        REQUIRE( c.x == 1.0 );
        REQUIRE( c.y == 5.0 );
        REQUIRE( c.z == 9.0 );
        REQUIRE( c.w == 13.0 );
        
        c = m.getRow(1);
        REQUIRE( c.x == 2.0 );
        REQUIRE( c.y == 6.0 );
        REQUIRE( c.z == 10.0 );
        REQUIRE( c.w == 14.0 );
        
        c = m.getRow(2);
        REQUIRE( c.x == 3.0 );
        REQUIRE( c.y == 7.0 );
        REQUIRE( c.z == 11.0 );
        REQUIRE( c.w == 15.0 );
        
        c = m.getRow(3);
        REQUIRE( c.x == 4.0 );
        REQUIRE( c.y == 8.0 );
        REQUIRE( c.z == 12.0 );
        REQUIRE( c.w == 16.0 );
    }
    
    SECTION("set row") {
        Matrix4 n;
        n.setRow(0, Vector4(1.0, 2.0, 3.0, 4.0));
        n.setRow(1, Vector4(5.0, 6.0, 7.0, 8.0));
        n.setRow(2, Vector4(9.0, 10.0, 11.0, 12.0));
        n.setRow(3, Vector4(13.0, 14.0, 15.0, 16.0));
        double a[16] = {
            1.0, 5.0, 9.0, 13.0,
            2.0, 6.0, 10.0, 14.0,
            3.0, 7.0, 11.0, 15.0,
            4.0, 8.0, 12.0, 16.0
        };
        Matrix4RequireEqual(n, a);
    }
}

TEST_CASE("Matrix4 invert", "[Matrix4]") {
    Matrix4 m0(
        1.0, 2.0, 3.0, 4.0,
        8.0, 6.0, 3.0, 8.0,
        9.0, 10.0, -3.0, 4.0,
        13.0, 4.0, -15.0, 6.0
    );
    
    Matrix4 m1 = m0;
    m1.invert();
    Matrix4 m = m0 * m1;
    Matrix4RequireIdentityAprox(m, kTestEPS);
}

TEST_CASE("Matrix4 transpose", "[Matrix4]") {
    Matrix4 m(
        1.0, 2.0, 3.0, 4.0,
        5.0, 6.0, 7.0, 8.0,
        9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    );
    m.transpose();
    
    double a[16] = {
        1.0, 5.0, 9.0, 13.0,
        2.0, 6.0, 10.0, 14.0,
        3.0, 7.0, 11.0, 15.0,
        4.0, 8.0, 12.0, 16.0
    };
    Matrix4RequireEqual(m, a);
}

TEST_CASE("Matrix4 transforms", "[Matrix4]") {
    SECTION("translate") {
        Matrix4 m;
        m.translate(1.0, 2.0, 3.0);
        REQUIRE( m.m30 == 1.0 );
        REQUIRE( m.m31 == 2.0 );
        REQUIRE( m.m32 == 3.0 );
        m.m30 = 0.0;
        m.m31 = 0.0;
        m.m32 = 0.0;
        Matrix4RequireIdentity(m);
    }
    
    SECTION("rotate") {
        Matrix4 m;
        double isqrt2 = 1.0 / M_SQRT2;
        m.rotate(M_PI * 0.5, -isqrt2, 0.0, isqrt2);
        Vector3 v(isqrt2, 0.0, isqrt2);
        v = Matrix4::transformV3(m, v);
        REQUIRE( v.x == Approx(0.0).margin(kTestEPS) );
        REQUIRE( v.y == Approx(1.0).margin(kTestEPS) );
        REQUIRE( v.z == Approx(0.0).margin(kTestEPS) );
    }
    
    SECTION("scale") {
        Matrix4 m;
        m.scale(0.5, 2.0, 0.25);
        REQUIRE( m.m00 == 0.5 );
        REQUIRE( m.m11 == 2.0 );
        REQUIRE( m.m22 == 0.25 );
        m.m00 = 1.0;
        m.m11 = 1.0;
        m.m22 = 1.0;
        Matrix4RequireIdentity(m);
    }
}

//TEST_CASE("view matrix", "[Matrix4]") {
//    SECTION("ortho") {
//        // TODO
//    }
//
//    SECTION("frustum") {
//        // TODO
//    }
//
//    SECTION("perspective") {
//        // TODO
//    }
//
//    SECTION("look at") {
//        // TODO
//    }
//}

TEST_CASE("MAtrix4 with Vector") {
    SECTION("with Vector3") {
        Matrix4 m = Matrix4::makeTranslation(1.0, 2.0, 3.0);
        m.rotate(M_PI * 0.5, 0.0, 1.0, 0.0);
        Vector3 v;
        
        v = Matrix4::transformV3(m, Vector3(1.0, 2.0, 3.0));
        REQUIRE( v.x == Approx(4.0).margin(kTestEPS) );
        REQUIRE( v.y == Approx(4.0).margin(kTestEPS) );
        REQUIRE( v.z == Approx(2.0).margin(kTestEPS) );
        
        v = Matrix4::mulV3(m, Vector3(1.0, 2.0, 3.0));
        REQUIRE( v.x == Approx(3.0).margin(kTestEPS) );
        REQUIRE( v.y == Approx(2.0).margin(kTestEPS) );
        REQUIRE( v.z == Approx(-1.0).margin(kTestEPS) );
        
        // This is not a suitable test.
        v = Matrix4::mulAndProjectV3(m, Vector3(1.0, 2.0, 3.0));
        REQUIRE( v.x == Approx(4.0).margin(kTestEPS) );
        REQUIRE( v.y == Approx(4.0).margin(kTestEPS) );
        REQUIRE( v.z == Approx(2.0).margin(kTestEPS) );
    }
    
    SECTION("with Vector4") {
        Matrix4 m = Matrix4::makeTranslation(1.0, 2.0, 3.0);
        Vector4 v = Matrix4::mulV4(m, Vector4(1.0, 2.0, 3.0, 4.0));
        REQUIRE( v.x == Approx(5.0).margin(kTestEPS) );
        REQUIRE( v.y == Approx(10.0).margin(kTestEPS) );
        REQUIRE( v.z == Approx(15.0).margin(kTestEPS) );
        REQUIRE( v.w == Approx(4.0).margin(kTestEPS) );
    }
}

TEST_CASE("Matrix4 operators", "[Matrix4]") {
    Matrix4 m0(
        1.0, 2.0, 3.0, 4.0,
        5.0, 6.0, 7.0, 8.0,
        9.0, 10.0, 11.0, 12.0,
        13.0, 14.0, 15.0, 16.0
    );
    Matrix4 m1(
        1.0, 2.0, 3.0, 4.0,
        2.0, 4.0, 5.0, 3.0,
        3.0, 6.0, 7.0, 2.0,
        4.0, 8.0, 9.0, 1.0
    );
    
    SECTION("additive") {
        Matrix4 m = m0 + m1;
        double a[16] = {
            2.0, 4.0, 6.0, 8.0,
            7.0, 10.0, 12.0, 11.0,
            12.0, 16.0, 18.0, 14.0,
            17.0, 22.0, 24.0, 17.0
        };
        Matrix4RequireEqualAprox(m, a, kTestEPS);
    }
    
    SECTION("subtruct") {
        Matrix4 m = m0 - m1;
        double a[16] = {
            0.0, 0.0, 0.0, 0.0,
            3.0, 2.0, 2.0, 5.0,
            6.0, 4.0, 4.0, 10.0,
            9.0, 6.0, 6.0, 15.0
        };
        Matrix4RequireEqualAprox(m, a, kTestEPS);
    }
    
    SECTION("multiply") {
        Matrix4 m = m0 * m1;
        double a[16] = {
            90.0, 100.0, 110.0, 120.0,
            106.0, 120.0, 134.0, 148.0,
            122.0, 140.0, 158.0, 176.0,
            138.0, 160.0, 182.0, 204.0
        };
        Matrix4RequireEqualAprox(m, a, kTestEPS);
    }
    
    SECTION("multiply scalar") {
        Matrix4 m = m0 * 2.0;
        double a[16] = {
            2.0, 4.0, 6.0, 8.0,
            10.0, 12.0, 14.0, 16.0,
            18.0, 20.0, 22.0, 24.0,
            26.0, 28.0, 30.0, 32.0
        };
        Matrix4RequireEqualAprox(m, a, kTestEPS);
    }
}
