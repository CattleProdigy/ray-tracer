#include <Eigen/Core>
#include "gtest/gtest.h"
#include "sphere.hpp"
#include "color.hpp"
#include "ray.hpp"

TEST(sphere_test, intersection_1) {
    // Unit Sphere
    Color col(0,0,0);
    Sphere unit(1.0, Eigen::Vector3f(0,0,0), col); 
    Ray r(Eigen::Vector3f(0,0,-5), Eigen::Vector3f(0,0,1));
    EXPECT_EQ(true, unit.hit(r, 0.0, 99999.0));
}

TEST(sphere_test, miss_intersection_1) {
    // Unit Sphere
    Color col(0,0,0);
    Sphere unit(1.0, Eigen::Vector3f(0,0,0), col); 
    Ray r2(Eigen::Vector3f(0,0,-5), Eigen::Vector3f(1,0,0));
    EXPECT_EQ(false, unit.hit(r2, 0.0, 99999.0));
}

TEST(sphere_test, intersection_2) {
    // Unit Sphere
    Color col(0,0,0);
    Sphere unit(1.0, Eigen::Vector3f(85.00,-20.30,90.0), col); 
    Ray r(Eigen::Vector3f(80.00,-20.30,90.0), Eigen::Vector3f(1,0,0));
    EXPECT_EQ(true, unit.hit(r, 0.0, 99999.0));
}

TEST(sphere_test, miss_intersection_2) {
    // Unit Sphere
    Color col(0,0,0);
    Sphere unit(1.0, Eigen::Vector3f(85.00,-20.30,90.0), col); 
    Ray r(Eigen::Vector3f(80.00,-20.30,90.0), Eigen::Vector3f(0,0.40,0));
    EXPECT_EQ(false, unit.hit(r, 0.0, 99999.0));
}
