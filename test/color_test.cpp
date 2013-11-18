#include "color.hpp"
#include <Eigen/Core>
#include "gtest/gtest.h"

TEST(ColorTest, value_constructor1) {
    color vc1(0.4, 0.5, 0.6);
    EXPECT_FLOAT_EQ(0.4, vc1.rgb[0]);
    EXPECT_FLOAT_EQ(0.5, vc1.rgb[1]);
    EXPECT_FLOAT_EQ(0.6, vc1.rgb[2]);
}

TEST(ColorTest, value_constructor2) {
    Eigen::Vector3f rgb(0.4, 0.5, 0.6);
    color vc1(rgb);
    EXPECT_FLOAT_EQ(0.4, vc1.rgb[0]);
    EXPECT_FLOAT_EQ(0.5, vc1.rgb[1]);
    EXPECT_FLOAT_EQ(0.6, vc1.rgb[2]);
}

TEST(ColorTest, copy_constructor) {
    color cc1(0.4, 0.5, 0.6);
    color cc2(cc1);
    EXPECT_FLOAT_EQ(0.4, cc2.rgb[0]);
    EXPECT_FLOAT_EQ(0.5, cc2.rgb[1]);
    EXPECT_FLOAT_EQ(0.6, cc2.rgb[2]);
}

TEST(ColorTest, add) {
    color add1(0.4, 0.5, 0.6);
    color add2(-0.6, 0.5, -0.4);
    color res = add1 + add2;
    EXPECT_FLOAT_EQ(-0.2, res.rgb[0]);
    EXPECT_FLOAT_EQ(1.0, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.2, res.rgb[2]);
}

TEST(ColorTest, multi) {
    color multi1(0.4, 0.5, 0.6);
    color multi2(-0.6, 0.5, 0.0);
    color res = multi1 * multi2;
    EXPECT_FLOAT_EQ(-0.24, res.rgb[0]);
    EXPECT_FLOAT_EQ(0.25, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(ColorTest, divide) {
    color div1(0.4, 0.5, 0.0);
    color div2(-0.6, 0.5, 0.6);
    color res = div1 / div2;
    EXPECT_FLOAT_EQ(0.4/-0.6, res.rgb[0]);
    EXPECT_FLOAT_EQ(1.0, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(ColorTest, scal_multi_r) {
    color multi(0.4, -0.5, 0.0);
    color res = multi * 0.5; 
    EXPECT_FLOAT_EQ(0.2, res.rgb[0]);
    EXPECT_FLOAT_EQ(-0.25, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(ColorTest, scal_multi_l) {
    color multi(0.4, -0.5, 0.0);
    color res = 0.5 * multi; 
    EXPECT_FLOAT_EQ(0.2, res.rgb[0]);
    EXPECT_FLOAT_EQ(-0.25, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(ColorTest, scal_div_l) {
    color div(0.4, -0.5, 0.0);
    color res = div / 0.5; 
    EXPECT_FLOAT_EQ(0.8, res.rgb[0]);
    EXPECT_FLOAT_EQ(-1.0, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(ColorTest, unary_pos) {
    color col(0.4, -0.5, 0.0);
    color res = +col;
    EXPECT_FLOAT_EQ(0.4, res.rgb[0]);
    EXPECT_FLOAT_EQ(-0.5, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(ColorTest, unary_neg) {
    color col(0.4, -0.5, 0.0);
    color res = -col;
    EXPECT_FLOAT_EQ(-0.4, res.rgb[0]);
    EXPECT_FLOAT_EQ(0.5, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(ColorTest, assignment) {
    color col(0.4, -0.5, 0.0);
    color res = col;
    EXPECT_FLOAT_EQ(0.4, res.rgb[0]);
    EXPECT_FLOAT_EQ(-0.5, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(ColorTest, add_assign) {
    color col(0.4, -0.5, 0.0);
    color add(0.1, -0.1, -0.1);
    col += add;
    EXPECT_FLOAT_EQ(0.5, col.rgb[0]);
    EXPECT_FLOAT_EQ(-0.6, col.rgb[1]);
    EXPECT_FLOAT_EQ(-0.1, col.rgb[2]);
}

TEST(ColorTest, minus_assign) {
    color col(0.4, -0.5, 0.0);
    color minus(0.1, -0.1, -0.1);
    col -= minus;
    EXPECT_FLOAT_EQ(0.3, col.rgb[0]);
    EXPECT_FLOAT_EQ(-0.4, col.rgb[1]);
    EXPECT_FLOAT_EQ(0.1, col.rgb[2]);
}

TEST(ColorTest, multi_assign) {
    color col(0.4, -0.5, 0.0);
    color multi(0.1, -0.1, -0.1);
    col *= multi;
    EXPECT_FLOAT_EQ(0.04, col.rgb[0]);
    EXPECT_FLOAT_EQ(0.05, col.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, col.rgb[2]);
}

TEST(ColorTest, div_assign) {
    color col(0.4, -0.5, 0.0);
    color div(0.1, -0.1, -0.1);
    col /= div;
    EXPECT_FLOAT_EQ(4.0, col.rgb[0]);
    EXPECT_FLOAT_EQ(5.0, col.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, col.rgb[2]);
}

TEST(ColorTest, to_pixel) {
    color col(1.0, 0.5, 0.0);
    Eigen::Vector3i res = col.to_pixel();
    EXPECT_EQ(255, res[0]);
    EXPECT_EQ(127, res[1]);
    EXPECT_EQ(0, res[2]);
}

TEST(ColorTest, to_pixel_sat) {
    color col(1.00010, 99999999999.9, -0.1);
    Eigen::Vector3i res = col.to_pixel();
    EXPECT_EQ(255, res[0]);
    EXPECT_EQ(255, res[1]);
    EXPECT_EQ(0, res[2]);
}

