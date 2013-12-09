#include <vector>
#include <cfloat>

#include "bounding_box.hpp"
#include "kd_mesh.hpp"
#include "mesh.hpp"
#include "triangle.hpp"

Bounding_box::Bounding_box(const std::vector<Triangle>& tris) {

    min = V3(FLT_MAX, FLT_MAX, FLT_MAX);
    max = V3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (const Triangle& tri : tris) {
        const std::vector<V3>& v = tri.m->verts;
        for (int i = 0; i < 3; ++i) {     // Vertex
            for (int j = 0; j < 3; ++j) { // Dimension
                if (v[tri.inds[i]][j] < min[j])
                    min[j] = v[tri.inds[i]][j];
                if (v[tri.inds[i]][j] > max[j])
                    max[j] = v[tri.inds[i]][j];
            }
        }
    }
}

Bounding_box::Bounding_box(const std::vector<Mesh*>& meshes) {

    min = V3(FLT_MAX, FLT_MAX, FLT_MAX);
    max = V3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (const Mesh* mesh: meshes) {
        for (const Triangle& tri : mesh->tris) {
            const std::vector<V3>& v = tri.m->verts;
            for (int i = 0; i < 3; ++i) {     // Vertex
                for (int j = 0; j < 3; ++j) { // Dimension
                    if (v[tri.inds[i]][j] < min[j])
                        min[j] = v[tri.inds[i]][j];
                    if (v[tri.inds[i]][j] > max[j])
                        max[j] = v[tri.inds[i]][j];
                }
            }
        }
    }
}

Bounding_box::Bounding_box(const std::vector<Kd_mesh>& meshes) {

    min = V3(FLT_MAX, FLT_MAX, FLT_MAX);
    max = V3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (const Kd_mesh& mesh: meshes) {
        for (const Triangle& tri : mesh.tris) {
            const std::vector<V3>& v = tri.m->verts;
            for (int i = 0; i < 3; ++i) {     // Vertex
                for (int j = 0; j < 3; ++j) { // Dimension
                    if (v[tri.inds[i]][j] < min[j])
                        min[j] = v[tri.inds[i]][j];
                    if (v[tri.inds[i]][j] > max[j])
                        max[j] = v[tri.inds[i]][j];
                }
            }
        }
    }
}
