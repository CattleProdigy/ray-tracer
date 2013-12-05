#include <iostream>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "mesh.hpp"
#include "ray.hpp"
#include "ray_tracer.hpp"
#include "triangle.hpp"
#include "sphere.hpp"

Triangle::Triangle(const Triangle& other) {
    inds = other.inds;
    normal = other.normal;
}

Triangle::Triangle(Mesh* m, const V3i& inds) {
    this->m = m;
    this->inds = inds;
}

Triangle& Triangle::operator=(const Triangle& other) {
    this->inds = other.inds;
    this->normal = other.normal;
    return (*this);
}

// Moeller-Trumbore
#define EPSILON 0.00001
bool Triangle::hit(const Ray& ray, const Ray_Tracer* rt,
                    float t_min, float t_max, Ray_Hit& rh) const {

    // Absolute Triangle Vertex Positions
    V3& v0 = m->verts[inds[0]].pos;
    V3& v1 = m->verts[inds[1]].pos;
    V3& v2 = m->verts[inds[2]].pos;

    // Two edges of triangle
    V3 e0, e1;
    e0 = v1 - v0;
    e1 = v2 - v0; 

    V3 p = ray.s.cross(e1);
    float deter = e0.dot(p);

    if (deter  > -EPSILON && deter < EPSILON)
        return false; 

    V3 t = ray.o - v0;
    
    // Store the inverse to reduce divisions
    float inv_deter = 1.0 / deter;
    float u = t.dot(p) * inv_deter;

    if (u < 0.0 || u > 1.0)
        return false;

    V3 q = t.cross(e0);
    float v = ray.s.dot(q) * inv_deter;

    if (v < 0.0 || u + v > 1.0)
        return false;

    float t_inter = e1.dot(q) * inv_deter;

    if (t_inter < t_min || t_inter > t_max)
        return false;

    rh.t = t_inter;
    rh.col = 0.1*m->mat.col;
    rh.normal = normal;
    rh.shape = m;

    if (ray.depth >= rt->depth_limit)
        return true;

/*
    if (is_light) {     
        rh.col = Color(1.0, 1.0, 1.0);
        return true;
    }
*/

    V3 int_loc = ray.at(t_inter);
    // Shadow
    float shade;
    for (Shape* light : rt->lights) {
        shade = 1.0f;

        // BIG ASSUMPTION THAT ALL LIGHTS ARE SPHERES
        Sphere* sph = static_cast<Sphere*>(light);

        // Make ray from intersection to light
        V3 int_to_light = sph->c - int_loc; 
        float dist_to_light = int_to_light.norm();
        int_to_light.normalize();
        Ray shadow_ray(int_loc + 0.00001f*int_to_light, int_to_light, ray.depth + 1);
        Ray_Hit shadow_hit;
        if (rt->trace(shadow_ray, 0.000001f, dist_to_light, shadow_hit, true)) {
            if (!shadow_hit.shape->is_light) {
                shade = 0.0;
                //std::cout << "occlusion" << std::endl;
                continue;
            }
        }

        float inner = int_to_light.dot(rh.normal);
        //std::cout << inner << std::endl;
        if (inner > 0.0) {
            rh.col += sph->mat.col *inner* 0.9 * shade * m->mat.col;
        }
    }

    // Reflection 
    V3 refl_dir = ray.s - 2.0f * (rh.normal.dot(ray.s)) * rh.normal;
    Ray refl_ray(int_loc + 0.00001f*refl_dir, refl_dir, ray.depth + 1);
    Ray_Hit refl_hit;
    if (rt->trace(refl_ray, 0.00001f, 99999999999, refl_hit, false)) {
        rh.col += 0.7*refl_hit.col * refl_hit.shape->mat.col;
    }

    return true;  
}
