#include <cmath>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <png++/png.hpp>
#include "ray.hpp"
#include "sphere.hpp"

Sphere::Sphere(float r, const V3& c, const Color& color, bool is_light) {
    this->r = r;
    this->c = c;
    this->col = color;
    this->is_light = is_light;
}

Sphere::Sphere(const Sphere& s) {
    this->r = s.r;
    this->c = s.c;
    this->col = s.col;
    this->is_light = s.is_light;
}

Sphere& Sphere::operator=(const Sphere& s) {
    this->r = s.r;
    this->c = s.c; 
    this->col = s.col;
    this->is_light = s.is_light;
    return *this;
}

bool Sphere::hit(const Ray& ray, float t_min, float t_max, Ray_Hit& rh) const {

    // Create a ray in object space (as if sphere is at origin)
    Ray obj_ray;
    obj_ray.s = ray.s;
    obj_ray.o = (ray.o - this->c);

    // An intersection happens if (O + tD)(O + tD) = r*r has real solns

    // Check determinant (non-negative => real solution(s) => intersection) 
    float a = obj_ray.s.dot(obj_ray.s);
    float b = 2 * obj_ray.s.dot(obj_ray.o);
    float c2 = obj_ray.o.dot(obj_ray.o) - r * r;

    float det = b * b - 4.0 * a * c2;

    // No real solutions
    if (det < 0.0) 
        return false;

    // Finish solving quadratic equation
    det = sqrt(det);

    float t;
    t = -(-b - det) / (2.0 * a);
    if (t < t_min)
        t = -(-b + det) / (2.0 * a);    

    if (t < t_min || t > t_max) {
        return false;
    }

    rh.t = t;
    rh.col = col;
    rh.normal = c - ray.at(t);
    rh.normal.normalize();

    return true;

}
