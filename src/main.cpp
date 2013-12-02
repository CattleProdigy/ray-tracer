#include <vector>
#include <random>
#include <string>
#include <iostream>

#include <Eigen/Core>
#include <png++/png.hpp>
#include <assimp/assimp.hpp>
#include <assimp/aiMesh.h>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>

#include "mesh.hpp"
#include "ray.hpp"
#include "ray_tracer.hpp"
#include "sphere.hpp"
#include "color.hpp"
#include "camera.hpp"

using V3 = Eigen::Vector3f;
using V2 = Eigen::Vector2f;

void gen_rand_samples(int bins_per_side, std::vector<V2>& points); 

int main (int argc, char ** argv) {

    if (argc < 2)
        return 1;
 
    std::string filename(argv[1]);

    Mesh m;
    m.load(filename);

    Camera cam(V3(0,1.41,1.41), V3(0,-1.41,-1.41), V3(0,1,0), 2.0, -2, -2, 2, 2); 
    Sphere s(.7, V3(0,0,0), Color(0,0,1), false);    
    Sphere luminare(.02, V3(-1.4,-1.9,0), Color(1.0, 1.0, 1.0), true);    

    Ray_Tracer rt(cam, 501, 501, Color(0.1, 0.1, 0.1), 3);
    rt.shapes.push_back(&s);
    rt.shapes.push_back(&m);
    rt.shapes.push_back(&luminare);

    rt.trace_all();
    rt.write_buffer("ray_image2.png");

    return 0;
}

void gen_rand_samples(int bins_per_side, std::vector<V2>& points) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0, 1);

    float bin_length = 1.0f / bins_per_side;

    float bin_x = 0.0;
    float bin_y = 0.0;
 
    for (int i = 0; i < bins_per_side; ++i) {    
        for (int j = 0; j < bins_per_side; ++j) {    
            float rand_num_unit = dis(gen);             
            bin_x = i * bin_length + bin_length*rand_num_unit - 0.5;
            rand_num_unit = dis(gen);             
            bin_y = j * bin_length + bin_length*rand_num_unit - 0.5;
            points.push_back(V2(bin_x, bin_y));
        } 
    } 
}
