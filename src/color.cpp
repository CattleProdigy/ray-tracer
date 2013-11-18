#include <Eigen/Core>
#include "color.hpp"

// ctors
color::color() {
    rgb = Eigen::Vector3f(0, 0, 0);
}

color::color(float red, float green, float blue) {
    rgb[0] = red;
    rgb[1] = green;
    rgb[2] = blue;
}

color::color(const color& other) {
    rgb = other.rgb;
}

color::color(const Eigen::Vector3f& rgb) {
    this->rgb = rgb;
}

// Binary Arithmetic Operators
color operator+(const color& col_l, const color& col_r) {
    return color(col_l.rgb + col_r.rgb);
}

color operator*(const color& col_l, const color& col_r) {
    return color(col_l.rgb.cwiseProduct(col_r.rgb));
}

color operator/(const color& col_l, const color& col_r) {
    return color(col_l.rgb.cwiseQuotient(col_r.rgb));
}

color operator*(const color& col_l, float scalar) {
    return color(col_l.rgb * scalar);
}

color operator*(float scalar, const color& col_r) {
    return color(col_r.rgb * scalar);
}

color operator/(const color& col_l, float scalar) {
    return color(col_l.rgb / scalar);
}

// Uniary Arithmetic
color color::operator+() const {
    return *this;
}

color color::operator-() const {
    return color(-rgb);
}

// Compound Assignment Arithmetic
color& color::operator=(const color& rhs) {
    rgb = rhs.rgb;
    return *this;
}

color& color::operator+=(const color& rhs) {
    rgb += rhs.rgb;
    return *this;
}

color& color::operator-=(const color& rhs) {
    rgb -= rhs.rgb;
    return *this;
}

color& color::operator*=(const color& rhs) {
    rgb = rgb.cwiseProduct(rhs.rgb);
    return *this;
}

color& color::operator/=(const color& rhs) {
    rgb = rgb.cwiseQuotient(rhs.rgb);
    return *this;
}

// Print
std::ostream& operator<<(std::ostream& os, const color& col) {
    os << "(R" << col.rgb[0] << 
          ",G" << col.rgb[1] <<
          ",B" << col.rgb[2] << std::endl;
    return os;
}

float color::r() const {
    return rgb[0];
}

float color::g() const {
    return rgb[1];
}

float color::b() const {
    return rgb[2];
}

Eigen::Vector3i color::to_pixel() const {
    Eigen::Vector3i rgb_pixel;
    for (int i = 0; i < 3; i++) {
        if (rgb[i] > 1.0)
            rgb_pixel[i] = 255;
        else if (rgb[i] < 0.0) 
            rgb_pixel[i] = 0;
        else
            rgb_pixel[i] = floor(255.0 * rgb[i]);
    }
    return rgb_pixel;
}

