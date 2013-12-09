#include "kd_tree.hpp"
#include "mesh.hpp"


Kd_tree::Kd_tree(char dim) {
    root = new Kd_tree_node;
    root->split_dim = 0;
    this->dim = dim;
}

void Kd_tree::add(Mesh* m) {
    meshes.push_back(m);
}

void Kd_tree::build() {
    build_tree(root, meshes); 
}

void Kd_tree::build_tree(Kd_tree_node * node, std::vector<Mesh *> meshes) {

    node->bbox = Bounding_box(meshes);
    // Stop Criteria
    if (meshes.size = 0) {
        node->is_leaf = true;
        node->meshes = new std::vector<Mesh *>;
        *node->meshes = meshes;
        return;
    }

    // Find Split Plane (Median distance (replace with SA heuristic))
    node->split_dist = (0.5) * 
                   (node->bbox.min[node->split_dim] + node->bbox.max[node->split_dim]);

    node->left  = new Kd_tree_node;
    node->left->split_dim = (node->split_dim + 1) % dim;
    node->left->is_leaf = false;
    node->left->bbox = node->bbox;
    node->left->bbox.max[node->split_dim] = split_dist;
    node->right = new Kd_tree_node;
    node->right->split_dim = (node->split_dim + 1) % dim;
    node->right->is_leaf = false;
    node->right->bbox = node->bbox;
    node->right->bbox.min[node->split_dim] = split_dist;

    // Partition the meshes onto either side of the splitting plane
    std::vector<Mesh *> right_meshes;
    for (auto itr = meshes.begin(); itr != meshes.end(); ++itr) {
        // Case 1 - Right side of splitting plane
        if ((*itr)->bbox.min[node->split_dim] > node->split_dist) {
            right_meshes.push_back(*itr);
            itr = meshes.erase(itr);
            if (itr == meshes.end())
               break;
            --itr;
        
        // Case 2 - Mesh straddles splitting plane
        } else if ((*itr)->bbox.max[node->split_dim] > node->split_dist && 
                    (*itr)->bbox.min[node->split_dim] <= node->split_dist){

            Mesh* right= new Mesh;




            }
            
    
        }
    }
        
    // Build 
    build_tree(node->left, meshes); 
    build_tree(node->right, right_meshes); 
}


/*
1. 
If we reach the stopping criteria then mark this as a leaf node

2.
Find the splitting plane for the current dimension

3.
Make left and right children, assign their dimensions, default to non-leaf

4.
Assign shapes left of the splitting plane to left and shapes right of the splitting plane to
the right. Split up meshs that straddle plane.

5. 
call on children

*/
