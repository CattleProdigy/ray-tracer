#include <vector>
#include <cfloat>

#include "kd_mesh.hpp"
#include "mesh.hpp"
#include "triangle.hpp"

Kd_mesh::Kd_mesh(const Mesh& m) {
    std::copy(m.tris.begin(), m.tris.end(), back_inserter(tris));
    bbox = Bounding_box(tris);
}

void Kd_mesh::add_tri(const Triangle &t) {
    tris.push_back(t);

    const std::vector<V3>& v = t.m->verts;
    for (int i = 0; i < 3; ++i) {     // Vertex
        for (int j = 0; j < 3; ++j) { // Dimension
            if (v[t.inds[i]][j] < bbox.min[j])
                bbox.min[j] = v[t.inds[i]][j];
            if (v[t.inds[i]][j] > bbox.max[j])
                bbox.max[j] = v[t.inds[i]][j];
        }
    }
}
