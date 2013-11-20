#ifndef COLOR_HPP
#define COLOR_HPP

#include <cstdio>
#include <iostream>
#include <Eigen/Core>

class Color {

    public:
        Color();
        ~Color() {};
        Color(float red, float green, float blue);
        Color(const Eigen::Vector3f& rgb);
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
        
        float r() const;
        float g() const;
        float b() const;
        Eigen::Vector3i to_pixel() const;

        Eigen::Vector3f rgb;
};

#endif
