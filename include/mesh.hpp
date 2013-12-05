#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <Eigen/Core>

#include "triangle.hpp"
#include "ray_hit.hpp"
#include "shape.hpp"

using V3 = Eigen::Vector3f;
using V3i = Eigen::Vector3i;

struct Vertex {
    Vertex () {};
    Vertex(const V3& _pos) 
      : pos(_pos) {}
    V3 pos;     // Position
    //V3 normal;  // Vertex normal 
};

class Mesh : public Shape {
    public:
        Mesh() {};
        ~Mesh() {};
        void load(std::string filename);
        virtual bool hit(Ray ray, const Ray_Tracer* rt, 
                         float t_min, float t_max, Ray_Hit& rh, bool shadow) const;

        std::vector<Vertex> verts;
        std::vector<Triangle> tris;
};

#endif
