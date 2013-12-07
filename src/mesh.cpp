#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <assimp/assimp.hpp>
#include <assimp/aiMesh.h>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>

#include "mesh.hpp"

using V3 = Eigen::Vector3f;

void Mesh::load(std::string filename) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, 
                                      aiProcess_Triangulate | 
                                      aiProcessPreset_TargetRealtime_Fast);

    struct aiMesh * mesh = scene->mMeshes[0];

    // Copy Vertices
    verts.resize(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        verts[i][0] = mesh->mVertices[i][0];
        verts[i][1] = mesh->mVertices[i][1];
        verts[i][2] = mesh->mVertices[i][2];
    }

    // Faces
    tris.resize(mesh->mNumFaces);
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];

        tris[i].m = this;

        if (face.mNumIndices != 3) {
            std::cerr << "Disgarding non-triangular face" << std::endl;
            continue;
        }

        tris[i].inds[0] = face.mIndices[0];
        tris[i].inds[1] = face.mIndices[1];
        tris[i].inds[2] = face.mIndices[2];

        // Calculate triangle's normal vector
        const V3& v1 = verts[tris[i].inds[1]] - verts[tris[i].inds[0]];
        const V3& v2 = verts[tris[i].inds[2]] - verts[tris[i].inds[0]];
        tris[i].normal = v1.cross(v2);
        tris[i].normal.normalize();
    }

    mat = Material(Color(0.2, 0.8, 0.2), 0.3, 0.7);
}

bool Mesh::hit(Ray ray, const Ray_Tracer* rt, 
               float t_min, float t_max, Ray_Hit& rh, bool shadow) const {

    bool hit = false;
    if (shadow) {
        for (const Triangle& i : tris) {
            if (i.hit(ray, rt, t_min, t_max, rh, true)) {
                return true;
            }
        }
    } else { 
        for (const Triangle& i : tris) {
            if (i.hit(ray, rt, t_min, t_max, rh, false)) {
                hit = true;
                t_max = rh.t;
            }
        }
    }
    return hit;
}
