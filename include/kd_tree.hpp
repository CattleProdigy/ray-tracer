#ifndef KD_TREE_HPP
#define KD_TREE_HPP

#include <vector>

#include "triangle.hpp"
#include "mesh.hpp"

typedef struct Kd_tree_node Kd_tree_node;

struct Kd_tree_node {
    union {
        struct {
            Kd_tree_node* left;    
            Kd_tree_node* right;    
        };
        std::vector<Mesh>* meshes; 
    };
    Bounding_box bbox;
    bool is_leaf;
    char split_dim;
    float split_dist;  // Distance of the spliting plane from the axis (whichever one)
};

class Kd_tree {

    public:
        Kd_tree(char dim);
        void add(Mesh* m);
        void build(); 
        void build_tree(Kd_tree_node* node, std::vector<Mesh * > meshs);
        
        Kd_tree_node* root;
        std::vector<Mesh *> meshes;
        std::vector<V3> verts;
        char dim;
};

#endif
