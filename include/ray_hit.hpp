#ifndef RAY_HIT_HPP
#define RAY_HIT_HPP

#include <Eigen/Core>
#include <color.hpp>

using V3 = Eigen::Vector3f;

struct Ray_Hit {
    float t;
    V3 normal;
    Color col;
};

#endif
