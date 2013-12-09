#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <Eigen/Core>
#include "ray.hpp"

// Copy Constructor
Ray::Ray(const Ray& r) {
    this->o = r.o;
    this->s = r.s;
    this->depth = r.depth;
}

// Value Constructor
Ray::Ray(V3 offset, V3 slope, unsigned short depth) {
    this->o = offset;
    this->s = slope;
    this->depth = depth;
}

// Assignment Operator
Ray& Ray::operator=(const Ray& r) {
    this->o = r.o;
    this->s = r.s;
    this->depth = r.depth;
    return *this;
}

V3 Ray::at(float t) const {
    return o + t*s;
}

// Print
std::ostream& operator<<(std::ostream& os, const Ray& r) {
    os << "(" << r.o[0] << ", "  << 
                 r.o[1] << ", "  << 
                 r.o[2] << ") (" << 
                 r.s[0] << ", "  << 
                 r.s[1] << ", "  << 
                 r.s[2] << ") "  << std::endl;
    
    return os;
}
