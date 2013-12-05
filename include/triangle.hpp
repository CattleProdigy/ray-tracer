#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <iostream>
#include <Eigen/Core>

#include <color.hpp>
#include <ray.hpp>
#include <ray_hit.hpp>

using V3 = Eigen::Vector3f;
using V3i = Eigen::Vector3i;

class Mesh;
class Ray_Tracer;

class Triangle {

    public:
        Triangle() {};
        ~Triangle() {};
        Triangle(const Triangle& other);
        Triangle(Mesh* m, const V3i& inds);
        Triangle& operator=(const Triangle& other);
        bool hit(const Ray& r, const Ray_Tracer* rt, 
                    float t_min, float t_max, Ray_Hit& rh) const;

        Mesh *m; 
        V3i inds;
        V3 normal;
};

#endif
