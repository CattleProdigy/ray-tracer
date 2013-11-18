#include <Eigen/Core>
#include <png++/png.hpp>
#include "ray.hpp"
#include "sphere.hpp"

Sphere::Sphere(float r, const Eigen::Vector3f& ccolor) {
    this->r = r;
    this->c = c;
}

Sphere::Sphere(const Sphere& s) {
    this->r = s.r;
    this->c = s.c;
}

Sphere& Sphere::operator=(const Sphere& s) {
    this->r = s.r;
    this->c = s.c; 
    return *this;
}
