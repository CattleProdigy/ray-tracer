#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "ray.hpp"
#include "ray_hit.hpp"

class Shape {
    public:
        Shape() {};
        virtual ~Shape() {};
        virtual bool hit(const Ray& r, float t_min, float t_max, Ray_Hit& rh) const = 0;

        bool is_light;
};

#endif
