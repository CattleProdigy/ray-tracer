#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <Eigen/Core>
#include <png++/png.hpp>

#include "color.hpp"
#include "ray.hpp"
#include "ray_hit.hpp"
#include "shape.hpp"

using V3 = Eigen::Vector3f;

class Sphere : public Shape {

    public:
        Sphere() {}; 
        ~Sphere() {};
        Sphere(float r, const V3& c, const Color& color, bool is_light);
        Sphere(const Sphere& s);
        Sphere& operator=(const Sphere& s);
        virtual bool hit(const Ray& r, float t_min, float t_max, Ray_Hit& rh) const; 

        V3 c;
        float r; 
        Color col;
};

#endif
