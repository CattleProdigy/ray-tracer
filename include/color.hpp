#include <cstdio>
#include <iostream>
#include <Eigen/Core>

class color {

    public:
        color();
        ~color() {};
        color(float red, float green, float blue);
        color(const Eigen::Vector3f& rgb);
        color(const color& other);

        // Binary Arithmetic Operators
        friend color operator+(const color& col_l, const color& col_r);
        friend color operator*(const color& col_l, const color& col_r);
        friend color operator/(const color& col_l, const color& col_r);
        friend color operator*(const color& col_l, float scalar);
        friend color operator*(float scalar, const color& col_r);
        friend color operator/(const color& col_l, float scalar);

        // Uniary Arithmetic
        color operator+() const;
        color operator-() const;

        // Compound Assignment Arithmetic
        color& operator=(const color& rhs);
        color& operator+=(const color& rhs);
        color& operator-=(const color& rhs);
        color& operator*=(const color& rhs);
        color& operator/=(const color& rhs);

        // Print
        friend std::ostream& operator<<(std::ostream& os, const color& col);
        
        float r() const;
        float g() const;
        float b() const;
        Eigen::Vector3i to_pixel() const;

        Eigen::Vector3f rgb;
};
