#include <Eigen/Core>
#include "gtest/gtest.h"
#include "sphere.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "ray_hit.hpp"
#include "shape.hpp"
#include "ray_tracer.hpp"
#include "camera.hpp"

using V3 = Eigen::Vector3f;

class sphere_test : public testing::Test {
    protected:  // You should make the members protected s.t. they can be
                // accessed from sub-classes.

        virtual void SetUp() {
            cam = Camera(V3(0,0,1), V3(0,0,-1), V3(0,1,0), 1.0, -2, -2, 2, 2); 
            rt = Ray_Tracer(cam, 501, 501, Color(0.1, 0.1, 0.1), 3, 4);
        }

        Camera cam;
        Ray_Tracer rt;
};

TEST_F(sphere_test, intersection_1) {
    // Unit Sphere
    Color col(0,0,0);
    Sphere unit(1.0, Eigen::Vector3f(0,0,0), col, false); 
    Ray r(Eigen::Vector3f(0,0,-5), Eigen::Vector3f(0,0,1), 0);
    Ray_Hit rh;
    EXPECT_TRUE(unit.hit(r, &rt, 0.0, 99999.0, rh));
    EXPECT_FLOAT_EQ(0.0, rh.normal[0]);
    EXPECT_FLOAT_EQ(0.0, rh.normal[1]);
    EXPECT_FLOAT_EQ(1.0, rh.normal[2]);
    EXPECT_FLOAT_EQ(4.0, rh.t);
}

TEST_F(sphere_test, miss_intersection_1) {
    // Unit Sphere
    Color col(0,0,0);
    Sphere unit(1.0, Eigen::Vector3f(0,0,0), col, false); 
    Ray r2(Eigen::Vector3f(0,0,-5), Eigen::Vector3f(1,0,0), 10);
    Ray_Hit rh;
    EXPECT_FALSE(unit.hit(r2,&rt, 0.0, 99999.0, rh));
}

TEST_F(sphere_test, intersection_2) {
    // Unit Sphere
    Color col(0,0,0);
    Sphere unit(1.0, Eigen::Vector3f(85.00,-20.30,90.0), col, false); 
    Ray r(Eigen::Vector3f(80.00,-20.23,90.0), Eigen::Vector3f(1,0,0), 10);
    Ray_Hit rh;
    EXPECT_TRUE(unit.hit(r,&rt, 0.0, 99999.0, rh));
}

TEST_F(sphere_test, miss_intersection_2) {
    // Unit Sphere
    Color col(0,0,0);
    Sphere unit(1.0, Eigen::Vector3f(85.00,-20.30,90.0), col, false); 
    Ray r(Eigen::Vector3f(80.00,-20.30,90.0), Eigen::Vector3f(0,0.40,0), 10);
    Ray_Hit rh;
    EXPECT_FALSE(unit.hit(r,&rt, 0.0, 99999.0, rh));
}
