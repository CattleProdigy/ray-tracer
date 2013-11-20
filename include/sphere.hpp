#ifndef SPHERE_HPP
#define SPHERE_HPP
#include <Eigen/Core>
#include <png++/png.hpp>
#include "ray.hpp"
#include "color.hpp"

class Sphere {

    public:
        Sphere() {}; 
        ~Sphere() {};
        Sphere(float r, const Eigen::Vector3f& c, const Color& color);
        Sphere(const Sphere& s);
        Sphere& operator=(const Sphere& s);
        bool hit(const Ray& r, float t_min, float t_max) const; 

        Eigen::Vector3f c;
        float r; 
        Color col;
};

#endif
