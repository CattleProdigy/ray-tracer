#include <iostream>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <mesh.hpp>
#include <ray.hpp>
#include <triangle.hpp>

Triangle::Triangle(const Triangle& other) {
    inds = other.inds;
    normal = other.normal;
    col = other.col;
}

Triangle::Triangle(Mesh* m, const V3i& inds, Color col) {
    this->m = m;
    this->inds = inds;
    this->col = col;
}

Triangle& Triangle::operator=(const Triangle& other) {
    this->inds = other.inds;
    this->normal = other.normal;
    this->col = other.col;
    return (*this);
}

// Moeller-Trumbore
#define EPSILON 0.00001
bool Triangle::hit(const Ray& r, float t_min, float t_max, Ray_Hit& rh) const {

    // Absolute Triangle Vertex Positions
    V3& v0 = m->verts[inds[0]].pos;
    V3& v1 = m->verts[inds[1]].pos;
    V3& v2 = m->verts[inds[2]].pos;

    // Two edges of triangle
    V3 e0, e1;
    e0 = v1 - v0;
    e1 = v2 - v0; 

    V3 p = r.s.cross(e1);
    float deter = e0.dot(p);

    if (deter  > -EPSILON && deter < EPSILON)
        return false; 

    V3 t = r.o - v0;
    
    // Store the inverse to reduce divisions
    float inv_deter = 1.0 / deter;
    float u = t.dot(p) * inv_deter;

    if (u < 0.0 || u > 1.0)
        return false;

    V3 q = t.cross(e0);
    float v = r.s.dot(q) * inv_deter;

    if (v < 0.0 || u + v > 1.0)
        return false;

    float t_inter = -1*e1.dot(q) * inv_deter;

    if (t_inter > EPSILON) {
        rh.t = t_inter;
        rh.col = col;
        rh.normal = normal;
        return true;
    }
   
    return false;
}
