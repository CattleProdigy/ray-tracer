#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "ray.hpp"
#include "ray_hit.hpp"

class Ray_Tracer;

class Shape {
    public:
        Shape() {};
        virtual ~Shape() {};
        virtual bool hit(Ray ray, const Ray_Tracer* rt, 
                         float t_min, float t_max, Ray_Hit& rh) const = 0;

        bool is_light;
        Color col;
};

#endif