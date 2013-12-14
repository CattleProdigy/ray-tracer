#include <cfloat>
#include <stack>

#include "mpi.h"

#include "kd_tree.hpp"
#include "kd_mesh.hpp"
#include "mesh.hpp"

#define EPSILON 0.00001

Kd_tree::Kd_tree(char dim, int np, int rank) {
    root = new Kd_tree_node;
    root->split_dim = 0;
    root->is_leaf = false;
    if (np > 1) {
        this->local_root_dim = (int)log2(np - 1);
        std::cout << local_root_dim << std::endl;
        std::cin.get();
    } else {
        this->local_root_dim = 2;
    }
            
    this->dim = dim;
    this->np  = np;
    this->rank = rank;
}

void Kd_tree::add(Mesh* m) {
    meshes.push_back(m);
}

void Kd_tree::build() {
    std::vector<Kd_mesh>* kd_meshes = new std::vector<Kd_mesh>;
    for (Mesh* mesh : meshes) {
        kd_meshes->push_back(Kd_mesh(*mesh));
    }
    build_tree(root, kd_meshes); 
    if (rank == 0)
        local_root = root;
}

void Kd_tree::build_tree(Kd_tree_node * node, std::vector<Kd_mesh>* meshes) {

    if (node->split_dim == local_root_dim) {
        if (local_roots.size() == (rank - 1)) {
            local_root = node;
        }
        local_roots.push_back(node);
    }
    int split_dim = node->split_dim % dim;

    std::cout << "Building tree" << std::endl;
    std::cout << "Dim = " << (int)split_dim << std::endl;
    int t = 0;
    for (Kd_mesh& kdm : *meshes) {
        t += kdm.tris.size();
    }
    std::cout << "Tri count = " << t << std::endl << std::endl;

    node->bbox = Bounding_box(*meshes);
    // Stop Criteria
    if (t < 150 || node->split_dim == 255) {
    //if (split_dim == 2) {
        node->is_leaf = true;
        node->kd_meshes = meshes;
        return;
    }

    // Find Split Plane (Median distance (replace with SA heuristic))
    node->split_dist = (0.5) * 
                   (node->bbox.min[split_dim] + node->bbox.max[split_dim]);
    std::cout << "Split_dist = " << node->split_dist << std::endl;
//    std::cin.get();

    node->left  = new Kd_tree_node;
    node->left->split_dim = (node->split_dim + 1);
    node->left->is_leaf = false;
    node->left->bbox = node->bbox;
    node->left->bbox.max[split_dim] = node->split_dist;
    node->right = new Kd_tree_node;
    node->right->split_dim = (node->split_dim + 1);
    node->right->is_leaf = false;
    node->right->bbox = node->bbox;
    node->right->bbox.min[split_dim] = node->split_dist;

    // Partition the meshes onto either side of the splitting plane
    std::vector<Kd_mesh>* right_kd_meshes = new std::vector<Kd_mesh>;
    for (auto itr = meshes->begin(); itr != meshes->end(); ++itr) {
        // Case 1 - Right side of splitting plane
        if (itr->bbox.min[split_dim] > node->split_dist) {

            // Move to right
            right_kd_meshes->push_back(*itr);

            // Remove from left
            itr = meshes->erase(itr);
            if (itr == meshes->end())
               break;
            --itr;
        
        
        // Case 2 - Mesh straddles splitting plane
        } else if (itr->bbox.max[split_dim] > node->split_dist && 
                    itr->bbox.min[split_dim] <= node->split_dist){

            Kd_mesh right;
            bool left_changed = false;

            for (auto tri = itr->tris.begin(); tri != itr->tris.end(); tri++) {

                // If the triangle is solely to the right of splitting plane
                if (tri->m->verts[tri->inds[0]][split_dim] > node->split_dist && 
                    tri->m->verts[tri->inds[1]][split_dim] > node->split_dist && 
                    tri->m->verts[tri->inds[2]][split_dim] > node->split_dist) {

                    // Move triangle from left to right
                    right.add_tri(*tri);
                    itr->tris.erase(tri);
                    left_changed = true;
                    if (tri == itr->tris.end())
                        break;
                    --tri;
                    
                // If the triangle straddle the splitting plane, duplicate it
                } else if(!(tri->m->verts[tri->inds[0]][split_dim] < node->split_dist
                    && tri->m->verts[tri->inds[1]][split_dim] < node->split_dist  
                    && tri->m->verts[tri->inds[2]][split_dim] < node->split_dist)) {
                    right.add_tri(*tri);
                } 
            }

            // Update the bounding box
            if (left_changed) {
                itr->bbox = Bounding_box(itr->tris);
            }
            right_kd_meshes->push_back(right);
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


bool Kd_tree_node::hit(Ray& ray, const Ray_Tracer* rt, float t_min, float t_max,
            Ray_Hit& rh, bool shadow) {

    bool hit = false;
    if (shadow) {
        for (const Kd_mesh& kd_mesh : *kd_meshes) {
            for (const Triangle& t : kd_mesh.tris) {
                if (t.hit(ray, rt, t_min, t_max, rh, true))
                    return true;
            }
        }
    } else {
        for (const Kd_mesh& kd_mesh : *kd_meshes) {
            for (const Triangle& t : kd_mesh.tris) {
                if (t.hit(ray, rt, t_min, t_max, rh, false)) {
                    t_max = rh.t;
                    hit = true;
                }
            }
        }
    }
    return hit;
}

bool Kd_tree::hit_helper(bool is_local, Kd_tree_node* node, Ray& ray, const Ray_Tracer* rt,
                         float t_min, float t_max, Ray_Hit& rh, bool shadow) {

    // Bounding box intersection
    V3 v0;
    v0[0] = node->bbox.min[0] - EPSILON;
    v0[1] = node->bbox.min[1] - EPSILON;
    v0[2] = node->bbox.min[2] - EPSILON;
    V3 v1;
    v1[0] = node->bbox.max[0] + EPSILON;
    v1[1] = node->bbox.max[1] + EPSILON;
    v1[2] = node->bbox.max[2] + EPSILON;

    float min = EPSILON;
    float max = FLT_MAX;

    /*
    std::cout<< " yo " << std::endl;
    std::cout<< ray.s << std::endl;
    std::cin.get();
    */
    for (int i = 0; i < 3; ++i) {
        if (fabs(ray.s[i]) > EPSILON) {
            float inv_slope = 1.0f / ray.s[i];
            if (inv_slope >= 0) {
                min = (v0[i] - ray.o[i]) * inv_slope;
                max = (v1[i] - ray.o[i]) * inv_slope;
            } else {
                min = (v1[i] - ray.o[i]) * inv_slope;
                max = (v0[i] - ray.o[i]) * inv_slope;
            }
        }
        if (min > t_min)
            t_min = min;
        if (max < t_max)
            t_max = max;
    }
    
    // Quit early if we don't even hit the bounding box
    if (t_min > t_max) {
        return false; 
    }

    // Havran's stack-based traversal
    int num_remote = 0;
    std::stack<Kd_tree_node*> node_stack;
    std::stack<float> t_stack;
    node_stack.push(node);
    t_stack.push(t_min);
    t_stack.push(t_max);
    bool hit_once = false;
    rh.t = FLT_MAX;

    while (!node_stack.empty() || num_remote > 0) {
        //std::cout << "dim: " << cur_node->split_dim << "  lrd: " << local_root_dim <<std::endl;

        if (num_remote > 0) {
            MPI_Status status;
            int flag;
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
            if (flag) {
                Ray_Hit rh_temp;
                MPI_Recv(&rh_temp, sizeof(Ray_Hit), MPI_BYTE, status.MPI_SOURCE,
                            status.MPI_TAG, MPI_COMM_WORLD, &status);
                //std::cout<< "Got a response" <<std::endl;
                if (rh.t > rh_temp.t) {
                    rh = rh_temp;
                    hit_once = true;
                }
                num_remote--;
            }
        }

        Kd_tree_node* cur_node;
        float curr_t_max;
        float curr_t_min;
        if (!node_stack.empty()) {
            cur_node = node_stack.top();
            node_stack.pop();
            curr_t_max = t_stack.top();
            t_stack.pop();
            curr_t_min = t_stack.top();
            t_stack.pop();
        } else {
            continue;
        }
         
        if (!is_local) {
            if (cur_node->split_dim == local_root_dim) {
                int pos = find(local_roots.begin(), local_roots.end(), cur_node) 
                                - local_roots.begin();
                Ray_Trace ray_trace;
                ray_trace.r = ray;
                ray_trace.t_min = curr_t_min;
                ray_trace.t_max = curr_t_max;
                ray_trace.shadow = shadow;
                num_remote++;
                MPI_Bsend(&ray_trace, sizeof(Ray_Trace), MPI_BYTE, 
                            pos + 1, pos, MPI_COMM_WORLD);
                continue;
            }
        }

        // If we hit a leaf just checking the primitives inside
        if (cur_node->is_leaf) {
            if (cur_node->hit(ray, rt, curr_t_min, curr_t_max, rh, shadow)){
                // TODO: Could this cause a problem?
                curr_t_max = rh.t;
                hit_once = true;
            }
        } else {

            int a = cur_node->split_dim % dim; // This we be used to index various vectors
            float entry = ray.o[a] + curr_t_min*ray.s[a];
            float exit  = ray.o[a] + curr_t_max*ray.s[a];
            float s     = cur_node->split_dist;

            if (entry <= s) {
                if (exit <= s) {
                    node_stack.push(cur_node->left);
                    t_stack.push(curr_t_min);
                    t_stack.push(curr_t_max);
                } else {
                    float t = (s - ray.o[a]) / ray.s[a];
                    node_stack.push(cur_node->left);
                    t_stack.push(curr_t_min);
                    t_stack.push(t);
                    node_stack.push(cur_node->right);
                    t_stack.push(t);
                    t_stack.push(curr_t_max);
                }
            } else {
                if (exit >= s) {
                    node_stack.push(cur_node->right);
                    t_stack.push(curr_t_min);
                    t_stack.push(curr_t_max);
                } else {
                    float t = (s - ray.o[a]) / ray.s[a];
                    node_stack.push(cur_node->right);
                    t_stack.push(curr_t_min);
                    t_stack.push(t);
                    node_stack.push(cur_node->left);
                    t_stack.push(t);
                    t_stack.push(curr_t_max);
                }
            }
        }
    }

    return hit_once;
}

bool Kd_tree::hit(Ray& ray, const Ray_Tracer* rt, float t_min, float t_max,
                    Ray_Hit& rh, bool shadow) {

    return hit_helper(false, root, ray, rt, t_min, t_max, rh, shadow);
}

bool Kd_tree::hit_local(Ray& ray, const Ray_Tracer* rt, float t_min, float t_max,
                    Ray_Hit& rh, bool shadow) {

    return hit_helper(true, local_root, ray, rt, t_min, t_max, rh, shadow);
}

