#include "color.hpp"
#include <Eigen/Core>
#include "gtest/gtest.h"

TEST(Color_test, value_constructor1) {
    Color vc1(0.4, 0.5, 0.6);
    EXPECT_FLOAT_EQ(0.4, vc1.rgb[0]);
    EXPECT_FLOAT_EQ(0.5, vc1.rgb[1]);
    EXPECT_FLOAT_EQ(0.6, vc1.rgb[2]);
}

TEST(Color_test, value_constructor2) {
    Eigen::Vector3f rgb(0.4, 0.5, 0.6);
    Color vc1(rgb);
    EXPECT_FLOAT_EQ(0.4, vc1.rgb[0]);
    EXPECT_FLOAT_EQ(0.5, vc1.rgb[1]);
    EXPECT_FLOAT_EQ(0.6, vc1.rgb[2]);
}

TEST(Color_test, copy_constructor) {
    Color cc1(0.4, 0.5, 0.6);
    Color cc2(cc1);
    EXPECT_FLOAT_EQ(0.4, cc2.rgb[0]);
    EXPECT_FLOAT_EQ(0.5, cc2.rgb[1]);
    EXPECT_FLOAT_EQ(0.6, cc2.rgb[2]);
}

TEST(Color_test, add) {
    Color add1(0.4, 0.5, 0.6);
    Color add2(-0.6, 0.5, -0.4);
    Color res = add1 + add2;
    EXPECT_FLOAT_EQ(-0.2, res.rgb[0]);
    EXPECT_FLOAT_EQ(1.0, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.2, res.rgb[2]);
}

TEST(Color_test, multi) {
    Color multi1(0.4, 0.5, 0.6);
    Color multi2(-0.6, 0.5, 0.0);
    Color res = multi1 * multi2;
    EXPECT_FLOAT_EQ(-0.24, res.rgb[0]);
    EXPECT_FLOAT_EQ(0.25, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(Color_test, divide) {
    Color div1(0.4, 0.5, 0.0);
    Color div2(-0.6, 0.5, 0.6);
    Color res = div1 / div2;
    EXPECT_FLOAT_EQ(0.4/-0.6, res.rgb[0]);
    EXPECT_FLOAT_EQ(1.0, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(Color_test, scal_multi_r) {
    Color multi(0.4, -0.5, 0.0);
    Color res = multi * 0.5; 
    EXPECT_FLOAT_EQ(0.2, res.rgb[0]);
    EXPECT_FLOAT_EQ(-0.25, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(Color_test, scal_multi_l) {
    Color multi(0.4, -0.5, 0.0);
    Color res = 0.5 * multi; 
    EXPECT_FLOAT_EQ(0.2, res.rgb[0]);
    EXPECT_FLOAT_EQ(-0.25, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(Color_test, scal_div_l) {
    Color div(0.4, -0.5, 0.0);
    Color res = div / 0.5; 
    EXPECT_FLOAT_EQ(0.8, res.rgb[0]);
    EXPECT_FLOAT_EQ(-1.0, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(Color_test, unary_pos) {
    Color col(0.4, -0.5, 0.0);
    Color res = +col;
    EXPECT_FLOAT_EQ(0.4, res.rgb[0]);
    EXPECT_FLOAT_EQ(-0.5, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(Color_test, unary_neg) {
    Color col(0.4, -0.5, 0.0);
    Color res = -col;
    EXPECT_FLOAT_EQ(-0.4, res.rgb[0]);
    EXPECT_FLOAT_EQ(0.5, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(Color_test, assignment) {
    Color col(0.4, -0.5, 0.0);
    Color res = col;
    EXPECT_FLOAT_EQ(0.4, res.rgb[0]);
    EXPECT_FLOAT_EQ(-0.5, res.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, res.rgb[2]);
}

TEST(Color_test, add_assign) {
    Color col(0.4, -0.5, 0.0);
    Color add(0.1, -0.1, -0.1);
    col += add;
    EXPECT_FLOAT_EQ(0.5, col.rgb[0]);
    EXPECT_FLOAT_EQ(-0.6, col.rgb[1]);
    EXPECT_FLOAT_EQ(-0.1, col.rgb[2]);
}

TEST(Color_test, minus_assign) {
    Color col(0.4, -0.5, 0.0);
    Color minus(0.1, -0.1, -0.1);
    col -= minus;
    EXPECT_FLOAT_EQ(0.3, col.rgb[0]);
    EXPECT_FLOAT_EQ(-0.4, col.rgb[1]);
    EXPECT_FLOAT_EQ(0.1, col.rgb[2]);
}

TEST(Color_test, multi_assign) {
    Color col(0.4, -0.5, 0.0);
    Color multi(0.1, -0.1, -0.1);
    col *= multi;
    EXPECT_FLOAT_EQ(0.04, col.rgb[0]);
    EXPECT_FLOAT_EQ(0.05, col.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, col.rgb[2]);
}

TEST(Color_test, div_assign) {
    Color col(0.4, -0.5, 0.0);
    Color div(0.1, -0.1, -0.1);
    col /= div;
    EXPECT_FLOAT_EQ(4.0, col.rgb[0]);
    EXPECT_FLOAT_EQ(5.0, col.rgb[1]);
    EXPECT_FLOAT_EQ(0.0, col.rgb[2]);
}

TEST(Color_test, to_pixel) {
    Color col(1.0, 0.5, 0.0);
    Eigen::Vector3i res = col.to_pixel();
    EXPECT_EQ(255, res[0]);
    EXPECT_EQ(127, res[1]);
    EXPECT_EQ(0, res[2]);
}

TEST(Color_test, to_pixel_sat) {
    Color col(1.00010, 99999999999.9, -0.1);
    Eigen::Vector3i res = col.to_pixel();
    EXPECT_EQ(255, res[0]);
    EXPECT_EQ(255, res[1]);
    EXPECT_EQ(0, res[2]);
}

