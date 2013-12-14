#ifndef KD_TREE_HPP
#define KD_TREE_HPP

#include <vector>

#include "triangle.hpp"
#include "kd_mesh.hpp"
#include "mesh.hpp"

//typedef struct Kd_tree_node Kd_tree_node;

class Kd_tree_node {
    public:
        Kd_tree_node() {};
        ~Kd_tree_node() {};
        bool hit(Ray& ray, const Ray_Tracer* rt, float t_min, float t_max,
                    Ray_Hit& rh, bool shadow);

        union {
            struct {
                Kd_tree_node* left;    
                Kd_tree_node* right;    
            };
            std::vector<Kd_mesh>* kd_meshes; 
        };
        Bounding_box bbox;
        bool is_leaf;
        unsigned char split_dim;
        float split_dist;  // Distance of the spliting plane from the axis (whichever one)
};

class Kd_tree {

    public:
        Kd_tree(char dim, int np, int rank);
        void add(Mesh* m);
        void build(); 
        void build_tree(Kd_tree_node* node, std::vector<Kd_mesh>* meshs);
        bool hit(Ray& ray, const Ray_Tracer* rt, float t_min, float t_max,
                    Ray_Hit& rh, bool shadow, unsigned int ray_id);
        bool hit_local(Ray& ray, const Ray_Tracer* rt, float t_min, float t_max,
                    Ray_Hit& rh, bool shadow);
        bool hit_helper(bool is_local, Kd_tree_node* node, Ray& ray, const Ray_Tracer* rt,
                         float t_min, float t_max, Ray_Hit& rh, 
                         bool shadow, unsigned int ray_id); 
        
        Kd_tree_node* root;
        Kd_tree_node* local_root;
        std::vector<Kd_tree_node*> local_roots;
        int local_root_dim;
        int np;
        int rank;
        std::vector<Mesh *> meshes;
        unsigned char dim;
};

#endif
