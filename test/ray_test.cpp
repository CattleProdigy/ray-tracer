#include <Eigen/Core>
#include "gtest/gtest.h"
#include "sphere.hpp"
#include "color.hpp"
#include "ray.hpp"

TEST(ray_test, value_constructor) {
    Eigen::Vector3f o(1.0, -1.3, 99999999.2);
    Eigen::Vector3f s(342.3, 0, 0);
    Ray r(o, s );
    EXPECT_FLOAT_EQ(o[0], r.o[0]); 
    EXPECT_FLOAT_EQ(o[1], r.o[1]); 
    EXPECT_FLOAT_EQ(o[2], r.o[2]); 
    EXPECT_FLOAT_EQ(s[0], r.s[0]); 
    EXPECT_FLOAT_EQ(s[1], r.s[1]); 
    EXPECT_FLOAT_EQ(s[2], r.s[2]); 
}


TEST(ray_test, copy_constructor) {
    Eigen::Vector3f o(1.0, -1.3, 99999999.2);
    Eigen::Vector3f s(342.3, 0, 0);
    Ray r(o, s );
    Ray r2(r);
    EXPECT_FLOAT_EQ(r.o[0], r2.o[0]); 
    EXPECT_FLOAT_EQ(r.o[1], r2.o[1]); 
    EXPECT_FLOAT_EQ(r.o[2], r2.o[2]); 
    EXPECT_FLOAT_EQ(r.s[0], r2.s[0]); 
    EXPECT_FLOAT_EQ(r.s[1], r2.s[1]); 
    EXPECT_FLOAT_EQ(r.s[2], r2.s[2]); 
}

TEST(ray_test, assignment_operator) {
    Eigen::Vector3f o(1.0, -1.3, 99999999.2);
    Eigen::Vector3f s(342.3, 0, 0);
    Ray r(o, s );
    Ray r2;
    r2 = r;
    EXPECT_FLOAT_EQ(r.o[0], r2.o[0]); 
    EXPECT_FLOAT_EQ(r.o[1], r2.o[1]); 
    EXPECT_FLOAT_EQ(r.o[2], r2.o[2]); 
    EXPECT_FLOAT_EQ(r.s[0], r2.s[0]); 
    EXPECT_FLOAT_EQ(r.s[1], r2.s[1]); 
    EXPECT_FLOAT_EQ(r.s[2], r2.s[2]); 
}
