#include <cmath>
#include <Eigen/Core>
#include <png++/png.hpp>
#include "ray.hpp"
#include "sphere.hpp"

Sphere::Sphere(float r, const Eigen::Vector3f& c, const Color& color) {
    this->r = r;
    this->c = c;
    this->col = color;
}

Sphere::Sphere(const Sphere& s) {
    this->r = s.r;
    this->c = s.c;
    this->col = s.col;
}

Sphere& Sphere::operator=(const Sphere& s) {
    this->r = s.r;
    this->c = s.c; 
    this->col = s.col;
    return *this;
}

bool Sphere::hit(const Ray& ray, float t_min, float t_max) const {

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

    float q;
    if (b > 0.0)
        q = -0.5 * (b + det);
    else
        q = -0.5 * (b - det);

    // Solutions
    float t0 = q / a;
    float t1 = c2 / q;    

    // Ensure the solutions are ordered
    if (t0 > t1) {
        float t_temp = t0;
        t0 = t1;
        t1 = t_temp;
    }

    return true;

}
