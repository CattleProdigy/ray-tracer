#include <iostream>
#include <random>
#include <string>
#include <vector>

#ifdef WITH_MPI
#include <mpi.h>
#endif

#include <Eigen/Core>
#include <png++/png.hpp>
#include <assimp/assimp.hpp>
#include <assimp/aiMesh.h>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>

#include "bounding_box.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "kd_tree.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "ray.hpp"
#include "ray_tracer.hpp"
#include "sphere.hpp"

using V3 = Eigen::Vector3f;
using V2 = Eigen::Vector2f;

int main (int argc, char ** argv) {

    #ifdef WITH_MPI
    MPI_Init(&argc, &argv);
    int id, np;
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    #endif

    if (argc < 2)
        return 1;

    std::string filename(argv[1]);

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, 
                                      aiProcess_Triangulate | 
                                      aiProcessPreset_TargetRealtime_MaxQuality);
    Kd_tree kdt(3, np, id);
    for (int i = 0; i < scene->mNumMeshes; ++i) {
        Mesh * m = new Mesh;
        m->from_aiMesh(scene->mMeshes[i]);

        kdt.add(m);
    }
    kdt.build();

    Material sm(Color(0.4, 0.4, 0.7),0.4, 1.0);
    Material lm(Color(0.6, 0.6, 0.6),1.0, 0.0);

    V3 pos(-62, -60, 70);
    V3 objcen(16.6117, 28.4714,-19.5600);

    // Dragon
    //V3 objcen(0, 5, 0);
    //V3 pos(1.6, 4.7, 26.2);
    //V3 objcen(0,0,0);
    //V3 pos(0,0,6);
    //V3 pos(0,2,2);
    Camera cam(pos, (objcen - pos).normalized(), V3(1,0,0), 1, -30, -30, 30, 30); 
    Sphere s(0.7, V3(0,0,0), sm, false);
    Sphere s2(0.3, V3(-.6,.8,0), sm, false);
    Sphere luminare(.02, V3(0.7,0.9,0), lm, true);
    Sphere luminare2(.02, V3(-0.7,-0.9,0), lm, true);

    Ray_Tracer rt(cam, 501, 501, Color(0.1, 0.1, 0.1), 3, 2);
    //rt.add_shape(&s);
    //rt.add_shape(&s2);
    //rt.add_shape(&m);
    rt.add_light(&luminare);
    rt.kd = &kdt;
    // rt.add_shape(&luminare2);

    rt.trace_all(id);
    rt.write_buffer("ray_image2.png");
    
    #ifdef WITH_MPI
    MPI_Finalize();
    #endif

    return 0;
}
