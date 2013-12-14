#ifndef RAY_HIT_HPP
#define RAY_HIT_HPP

#include <Eigen/Core>
#include "color.hpp"

using V3 = Eigen::Vector3f;

// forward declaration
class Shape;

struct Ray_Hit {
    float t;
    Color col;
    const Shape* shape;
};

#endif
