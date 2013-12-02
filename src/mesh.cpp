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
        verts[i].pos[0] = mesh->mVertices[i][0];
        verts[i].pos[1] = mesh->mVertices[i][1];
        verts[i].pos[2] = mesh->mVertices[i][2];
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
        const V3& v1 = verts[tris[i].inds[1]].pos - verts[tris[i].inds[0]].pos;
        const V3& v2 = verts[tris[i].inds[2]].pos - verts[tris[i].inds[0]].pos;
        tris[i].normal = v1.cross(v2);
        tris[i].normal.normalize();

        tris[i].col = Color(0, 0, 1.0); // Blue
        
    }
}

bool Mesh::hit(const Ray&r, float t_min, float t_max, Ray_Hit& rh) const {

    bool hit = false;

    for (const Triangle& i : tris) {
        if (i.hit(r, t_min, t_max, rh)) {
            hit = true;
            t_min = rh.t;
        }
    }

    rh.col = (0.1 + r.s.dot(rh.normal))*rh.col; 

    return hit;
}
