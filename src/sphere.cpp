#include <cmath>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <png++/png.hpp>
#include "ray.hpp"
#include "ray_tracer.hpp"
#include "sphere.hpp"

Sphere::Sphere(float r, const V3& c, const Material& mat, bool is_light) {
    this->r = r;
    this->c = c;
    this->mat = mat;
    this->is_light = is_light;
}

Sphere::Sphere(const Sphere& s) {
    this->r = s.r;
    this->c = s.c;
    this->mat = s.mat;
    this->is_light = s.is_light;
}

Sphere& Sphere::operator=(const Sphere& s) {
    this->r = s.r;
    this->c = s.c; 
    this->mat = s.mat;
    this->is_light = s.is_light;
    return *this;
}

bool Sphere::hit(Ray ray, const Ray_Tracer* rt, 
                        float t_min, float t_max, Ray_Hit& rh, bool shadow) const {

    // Create a ray in object space (as if sphere is at origin)
    Ray obj_ray;
    obj_ray.s = ray.s;
    obj_ray.o = ray.o - this->c;

    // An intersection happens if (O + tD)(O + tD) = r*r has real solns

    // Check determinant (non-negative => real solution(s) => intersection) 
    float a = obj_ray.s.dot(obj_ray.s);
    float b = 2.0 * obj_ray.s.dot(obj_ray.o);
    float c2 = obj_ray.o.dot(obj_ray.o) - r * r;

    float det = b * b - 4.0 * a * c2;

    // No real solutions
    if (det < 0.0) 
        return false;

    // Finish solving quadratic equation
    det = sqrt(det);

    float q;
    if (b > 0)
        q = -0.5 * (b + det);
    else 
        q = -0.5 * (b - det);

    float t = q / a;
    float t1 = c2 / q; 
    if (t > t1) {
        float temp = t;
        t = t1;
        t1 = temp;
    }

    if (t < t_min || t > t_max) {
        return false;
    }
    
    // Ambient
    rh.t = t;
    rh.col = 0.1*mat.col;
    rh.normal = c - ray.at(t);
    rh.normal.normalize();
    rh.shape = this;

    if (ray.depth >= rt->depth_limit)
        return true;

    if (is_light) {     
        rh.col = Color(1.0, 1.0, 1.0);
        return true;
    }

    if (shadow)
        return true;


    // Store location of ray-sphere intersection
    V3 int_loc = ray.at(t); 

    // Shadow
    float shade;
    for (Shape* light : rt->lights) {
        shade = 1.0f;

        // BIG ASSUMPTION THAT ALL LIGHTS ARE SPHERES
        Sphere* sph = static_cast<Sphere*>(light);

        // Make ray from intersection to light
        V3 int_to_light = int_loc - sph->c; 
        float dist_to_light = int_to_light.norm();
        int_to_light.normalize();
        Ray shadow_ray(int_loc + 0.1f*int_to_light, int_to_light, ray.depth + 1);
        Ray_Hit shadow_hit;
        if (rt->trace(shadow_ray, 0.00001f, dist_to_light, shadow_hit, true)) {
            if (!shadow_hit.shape->is_light) {
                shade = 0.0;
        //        std::cout << "occlusion" << std::endl;
                continue;
            }
        }

        float inner = int_to_light.dot(rh.normal);
        //std::cout << inner << std::endl;
        if (inner > 0.0) {
            rh.col += sph->mat.col * inner * mat.diff * shade * mat.col;
        }
    }

    // Reflection 
    V3 refl_dir = ray.s - 2.0f * (rh.normal.dot(ray.s)) * rh.normal;
    Ray refl_ray(int_loc + 0.00001f*refl_dir, refl_dir, ray.depth + 1);
    Ray_Hit refl_hit;
    if (rt->trace(refl_ray, 0.00001f, 99999999999, refl_hit, false)) {
        rh.col += mat.refl * refl_hit.col * refl_hit.shape->mat.col;
    }

    return true;

}

