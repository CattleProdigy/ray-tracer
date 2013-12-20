#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <Eigen/Core>

#include "ray.hpp"
#include "ray_hit.hpp"

using V3 = Eigen::Vector3f;

class Primitive {

    public:
        virtual bool hit(const Ray& r, const Ray_tracer& rt, float t_min,
                            float t_max, Ray_Hit& rh, bool shadow) const = 0;

    Model * model;
    V3 normal;
}

#endif
