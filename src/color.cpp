#include <Eigen/Core>
#include "color.hpp"

// ctors
Color::Color() {
    rgb = V3(0, 0, 0);
}

Color::Color(float red, float green, float blue) {
    rgb[0] = red;
    rgb[1] = green;
    rgb[2] = blue;
}

Color::Color(const Color& other) {
    rgb = other.rgb;
}

Color::Color(const V3& rgb) {
    this->rgb = rgb;
}

// Binary Arithmetic Operators
Color operator+(const Color& col_l, const Color& col_r) {
    return Color(col_l.rgb + col_r.rgb);
}

Color operator*(const Color& col_l, const Color& col_r) {
    return Color(col_l.rgb.cwiseProduct(col_r.rgb));
}

Color operator/(const Color& col_l, const Color& col_r) {
    return Color(col_l.rgb.cwiseQuotient(col_r.rgb));
}

Color operator*(const Color& col_l, float scalar) {
    return Color(col_l.rgb * scalar);
}

Color operator*(float scalar, const Color& col_r) {
    return Color(col_r.rgb * scalar);
}

Color operator/(const Color& col_l, float scalar) {
    return Color(col_l.rgb / scalar);
}

// Uniary Arithmetic
Color Color::operator+() const {
    return *this;
}

Color Color::operator-() const {
    return Color(-rgb);
}

// Compound Assignment Arithmetic
Color& Color::operator=(const Color& rhs) {
    rgb = rhs.rgb;
    return *this;
}

Color& Color::operator+=(const Color& rhs) {
    rgb += rhs.rgb;
    return *this;
}

Color& Color::operator-=(const Color& rhs) {
    rgb -= rhs.rgb;
    return *this;
}

Color& Color::operator*=(const Color& rhs) {
    rgb = rgb.cwiseProduct(rhs.rgb);
    return *this;
}

Color& Color::operator/=(const Color& rhs) {
    rgb = rgb.cwiseQuotient(rhs.rgb);
    return *this;
}

// Print
std::ostream& operator<<(std::ostream& os, const Color& col) {
    os << "(R" << col.rgb[0] << 
          ",G" << col.rgb[1] <<
          ",B" << col.rgb[2] << ')' << std::endl;
    return os;
}

float Color::r() const {
    return rgb[0];
}

float Color::g() const {
    return rgb[1];
}

float Color::b() const {
    return rgb[2];
}

V3i Color::to_pixel() const {
    V3i rgb_pixel;
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

png::rgb_pixel Color::to_png_pixel() const {
    V3i int_pixel = to_pixel();
    png::rgb_pixel rgb;
    rgb.red = int_pixel[0];
    rgb.green = int_pixel[1];
    rgb.blue = int_pixel[2];
    
    return rgb;
}
