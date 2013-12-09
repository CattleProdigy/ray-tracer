#ifndef KD_MESH_HPP
#define KD_MESH_HPP

#include <vector>

#include "bounding_box.hpp"
#include "triangle.hpp"
#include "mesh.hpp"


class Kd_mesh {
    public: 
        Kd_mesh() {};
        ~Kd_mesh() {};
        Kd_mesh(const Mesh& m);
        void add_tri(const Triangle& t);
        Bounding_box bbox;
        std::vector<Triangle> tris;
};

#endif
