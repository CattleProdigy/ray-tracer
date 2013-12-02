#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <iostream>
#include <Eigen/Core>

#include <ray.hpp>
#include <mesh.hpp>
#include <color.hpp>
#include <ray_hit.hpp>

using V3 = Eigen::Vector3f;
using V3i = Eigen::Vector3i;

class Mesh;

class Triangle {

    public:
        Triangle() {};
        ~Triangle() {};
        Triangle(const Triangle& other);
        Triangle(Mesh* m, const V3i& inds, Color col);
        Triangle& operator=(const Triangle& other);
        bool hit(const Ray& r, float t_min, float t_max, Ray_Hit& rh) const;

        Mesh *m; 
        V3i inds;
        V3 normal;
        Color col;
};

#endif
