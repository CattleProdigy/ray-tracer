#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdio>
#include <iostream>

#include <Eigen/Core>
#include <png++/png.hpp>

using V3 = Eigen::Vector3f;
using V3i = Eigen::Vector3i;

class Color {

    public:
        // ctors
        Color();
        ~Color() {};
        Color(float red, float green, float blue);
        Color(const V3& rgb);
        Color(const Color& other);

        // Binary Arithmetic Operators
        friend Color operator+(const Color& col_l, const Color& col_r);
        friend Color operator*(const Color& col_l, const Color& col_r);
        friend Color operator/(const Color& col_l, const Color& col_r);
        friend Color operator*(const Color& col_l, float scalar);
        friend Color operator*(float scalar, const Color& col_r);
        friend Color operator/(const Color& col_l, float scalar);

        // Uniary Arithmetic
        Color operator+() const;
        Color operator-() const;

        // Compound Assignment Arithmetic
        Color& operator=(const Color& rhs);
        Color& operator+=(const Color& rhs);
        Color& operator-=(const Color& rhs);
        Color& operator*=(const Color& rhs);
        Color& operator/=(const Color& rhs);

        // Print
        friend std::ostream& operator<<(std::ostream& os, const Color& col);
        
        // Accessors
        float r() const;
        float g() const;
        float b() const;
        V3i to_pixel() const;
        png::rgb_pixel to_png_pixel() const;

        V3 rgb;
};

#endif
