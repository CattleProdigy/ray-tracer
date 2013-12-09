#include "kd_tree.hpp"
#include "kd_mesh.hpp"
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
    std::vector<Kd_mesh> kd_meshes;
    for (Mesh * mesh : meshes) {
        kd_meshes.push_back(Kd_mesh(*mesh));
    }
    build_tree(root, kd_meshes); 
}

void Kd_tree::build_tree(Kd_tree_node * node, std::vector<Kd_mesh>& meshes) {

    /*
    std::cout << "Building tree" << std::endl;
    std::cout << "Dim = " << (int)node->split_dim << std::endl;
    int t = 0;
    for (Kd_mesh& kdm : meshes) {
        t += kdm.tris.size();
    }
    std::cout << "Tri count = " << t << std::endl << std::endl;
    std::cin.get();
    */


    node->bbox = Bounding_box(meshes);
    // Stop Criteria
    if (node->split_dim == 2) {
        node->is_leaf = true;
        node->kd_meshes = new std::vector<Kd_mesh>;
        *node->kd_meshes = meshes;
        return;
    }

    // Find Split Plane (Median distance (replace with SA heuristic))
    node->split_dist = (0.5) * 
                   (node->bbox.min[node->split_dim] + node->bbox.max[node->split_dim]);

    node->left  = new Kd_tree_node;
    node->left->split_dim = (node->split_dim + 1) % dim;
    node->left->is_leaf = false;
    node->left->bbox = node->bbox;
    node->left->bbox.max[node->split_dim] = node->split_dist;
    node->right = new Kd_tree_node;
    node->right->split_dim = (node->split_dim + 1) % dim;
    node->right->is_leaf = false;
    node->right->bbox = node->bbox;
    node->right->bbox.min[node->split_dim] = node->split_dist;

    // Partition the meshes onto either side of the splitting plane
    std::vector<Kd_mesh> right_kd_meshes;
    for (auto itr = meshes.begin(); itr != meshes.end(); ++itr) {
        // Case 1 - Right side of splitting plane
        if (itr->bbox.min[node->split_dim] > node->split_dist) {

            // Move to right
            right_kd_meshes.push_back(*itr);

            // Remove from left
            itr = meshes.erase(itr);
            if (itr == meshes.end())
               break;
            --itr;
        
        // Case 2 - Mesh straddles splitting plane
        } else if (itr->bbox.max[node->split_dim] > node->split_dist && 
                    itr->bbox.min[node->split_dim] <= node->split_dist){

            Kd_mesh right;
            bool left_changed = false;

            for (auto tri = itr->tris.begin(); tri != itr->tris.end(); tri++) {
                if (tri->m->verts[tri->inds[0]][node->split_dim] > node->split_dist && 
                    tri->m->verts[tri->inds[1]][node->split_dim] > node->split_dist && 
                    tri->m->verts[tri->inds[2]][node->split_dim] > node->split_dist) {

                    // Move triangle from left to right
                    right.add_tri(*tri);
                    itr->tris.erase(tri);
                    left_changed = true;
                    if (tri == itr->tris.end())
                        break;
                    --tri;
                    
                } else if(!(tri->m->verts[tri->inds[0]][node->split_dim] < node->split_dist
                    && tri->m->verts[tri->inds[1]][node->split_dim] < node->split_dist  
                    && tri->m->verts[tri->inds[2]][node->split_dim] < node->split_dist)) {
                    right.add_tri(*tri);
                } 
            }

            // Update the bounding box
            if (left_changed) {
                itr->bbox = Bounding_box(itr->tris);
            }

            right_kd_meshes.push_back(right);

        }
    }
        
    // Build 
    build_tree(node->left, meshes); 
    build_tree(node->right, right_kd_meshes); 
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
