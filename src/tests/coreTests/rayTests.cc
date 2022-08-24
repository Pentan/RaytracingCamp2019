#include <cmath>
#include <doctest.h>
#include "../testsupport.h"

#include <pinkycore/pptypes.h>
#include <pinkycore/ray.h>
#include <pinkycore/aabb.h>

using namespace PinkyPi;

namespace {

}


TEST_CASE("Ray transform test [Ray]") {
    Ray ray(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, -1.0));
    Quaterion q = Quaterion::makeRotation(M_PI * 0.25, 1.0, 0.0, 0.0);
    Matrix4 m = q.getMatrix();

    Vector3 p(0.0, 0.0, -1.0);
    Vector3 pm = Matrix4::transformV3(m, p);
    Vector3 pq = q.rotate(p);

    Ray ray1 = ray.transformed(m);
    REQUIRE(ray1.origin.x == doctest::Approx(ray.origin.x).epsilon(kTestEPS));
    REQUIRE(ray1.origin.y == doctest::Approx(ray.origin.y).epsilon(kTestEPS));
    REQUIRE(ray1.origin.z == doctest::Approx(ray.origin.z).epsilon(kTestEPS));

}