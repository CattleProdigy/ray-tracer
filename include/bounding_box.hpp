#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#include <vector>
#include <Eigen/Core>

#include "mesh.hpp"
#include "triangle.hpp"

using V3 = Eigen::Vector3f;

class Kd_mesh;

class Bounding_box {

    public:
        Bounding_box () {};
        ~Bounding_box () {};
        Bounding_box(const std::vector<Triangle>& tris);
        Bounding_box(const std::vector<Mesh*>& meshes);
        Bounding_box(const std::vector<Kd_mesh>& meshes);

        V3 min; 
        V3 max; 
};

#endif
