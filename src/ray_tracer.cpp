#include <vector>
#include <cfloat>
#include <png++/png.hpp>
#include <mpi.h>

#include "camera.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "ray_tracer.hpp"
#include "shape.hpp"
#include "sphere.hpp"

using V2 = Eigen::Vector2f;

#define EPSILON 0.00001
#define NOTHING_TO_SEND 32767

// Helper
static void gen_rand_samples(int bins_per_side, std::vector<V2>& points);

Ray_Tracer::Ray_Tracer(const Camera& cam, unsigned int x_res, unsigned int y_res,
              Color background_col, unsigned int sample_bins, unsigned int depth_limit) {
    this->cam = cam;
    this->x_res = x_res;
    this->y_res = y_res;
    image_buf.resize(x_res);
    for (std::vector<Color>& y_buf: image_buf) {
        y_buf.resize(y_res, Color(0,0,0));
    }

    this->background_col = background_col;
    this->sample_bins = sample_bins;
    this->depth_limit = depth_limit;
}

Ray_Tracer& Ray_Tracer::operator=(const Ray_Tracer& rt) {

    this->cam = rt.cam;
    this->x_res = rt.x_res;
    this->y_res = rt.y_res;
    image_buf.resize(x_res);
    for (std::vector<Color>& y_buf: image_buf)
        y_buf.resize(y_res);

    this->background_col = rt.background_col;
    this->sample_bins = rt.sample_bins;
    this->depth_limit = rt.depth_limit;

    return *this;
}

void Ray_Tracer::add_shape(Shape* shape) {
    shapes.push_back(shape);
    if (shape->is_light) 
        lights.push_back(shape);
}

void Ray_Tracer::add_light(Sphere* sphere) {
    shapes.push_back(sphere);
    if (sphere->is_light) 
        lights.push_back(sphere);
}

void Ray_Tracer::free_shapes() {
    for (Shape* shape : shapes)
        delete shape;
}

bool Ray_Tracer::trace(const Ray& r, float t_min, float t_max, 
                        Ray_Hit& rh, bool shadow) const {
    bool hit_once = false;
    Ray r2 = r;
    return kd->hit_local(r2, this, t_min, t_max, rh, shadow);
    return hit_once;
}

void Ray_Tracer::worker() {

    MPI_Barrier(MPI_COMM_WORLD);
    std::stack<Ray_Trace> rts;
    std::vector<Ray_Hit_Remote> rhrs;
    while (true) {
        MPI_Status status;
        int flag;
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        if (flag) {
            Ray_Trace rt;
            MPI_Recv(&rt, sizeof(Ray_Trace), MPI_BYTE, MPI_ANY_SOURCE,
                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            if (rt.t_min == -1 && rt.t_max == -1)
                break;

            rts.push(rt);


        // Removed because interleaving comms and work is slow
        //    continue;
        } /*else {
            if (!rts.empty()) {
                Ray_Trace rt = rts.top();
                rts.pop();
                Ray_Hit rh;
                Ray_Hit_Remote rhr;
                if (!(kd->hit_local(rt.r, this, rt.t_min, rt.t_max, rh, false))) {
                    rhr.t = FLT_MAX;
                } else {
                    rhr.t = rh.t;
                    rhr.col = rh.col;
                    rhr.ray_id = rt.ray_id;
                    rhrs.push_back(rhr);
                }
            }
        }
        */
    }

    std::cout << "Just processing now" << std::endl;
    
    // Process remaining rays
    while (!rts.empty()) {
        Ray_Trace rt = rts.top();
        rts.pop();
        Ray_Hit rh;
        Ray_Hit_Remote rhr;
        if (!(kd->hit_local(rt.r, this, rt.t_min, rt.t_max, rh, false))) {
            rhr.t = FLT_MAX;
        } else {
            rhr.t = rh.t;
            rhr.col = rh.col;
            rhr.ray_id = rt.ray_id;
            rhrs.push_back(rhr);
        }
    }

    std::cout << "Done processing, gonna send now" << std::endl;

    if (!rhrs.empty()) {
        // Send back ray info
        MPI_Ssend(rhrs.data(), sizeof(rhrs[0]) * rhrs.size(), 
                    MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    } else {
        // Send a message staying we're done, but have nothing to sedn
        unsigned int done = 0xFFFFFFFF;
        MPI_Ssend(&done, 1, MPI_INT, 0, NOTHING_TO_SEND, MPI_COMM_WORLD);
    }

/*
    MPI_Barrier(MPI_COMM_WORLD);
    while (true) {
        Ray_Trace rt;
        MPI_Status status;
        MPI_Recv(&rt, sizeof(Ray_Trace), MPI_BYTE, MPI_ANY_SOURCE,
                     MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if (rt.t_min == -1 && rt.t_max == -1)
            break;
        

        Ray_Hit rh;
        Ray_Hit_Remote rhr;
        if (!(kd->hit_local(rt.r, this, rt.t_min, rt.t_max, rh, false))) {
            rhr.t = FLT_MAX;
        } else {
            rhr.t = rh.t;
        }

        rhr.col = rh.col;
        
        MPI_Send(&rhr, sizeof(Ray_Hit_Remote), MPI_BYTE, status.MPI_SOURCE,
                    status.MPI_TAG, MPI_COMM_WORLD);
    }
*/
}

void Ray_Tracer::trace_all(int rank, int np) {

    std::cout << "hi" << std::endl;
    if (rank != 0) {
        worker();
        return;
    }

    std::vector<V2> points;
    MPI_Barrier(MPI_COMM_WORLD);
    for (unsigned int i = 0; i < x_res; ++i) {
        for (unsigned int j = 0; j < y_res; ++j) {
            points.clear();
            gen_rand_samples(sample_bins, points);

            Color total;
            for (unsigned int k = 0; k < points.size(); ++k) {
                V2& p = points[k];
                Ray r = cam.make_ray((i + p.x() + 0.5)/x_res, (j + p.y() + 0.5)/y_res);
                local_rays.push(r);
                local_dest.push(Int_pair(i, j));
                Ray_Hit rh;
                unsigned int ray_id = i << 19 | j << 6 | k;
                kd->hit(r, this, EPSILON, FLT_MAX, rh, false, ray_id);
                //trace(r, EPSILON, FLT_MAX, rh, false);
                    //total += rh.col;
            }
//            image_buf[i][j] = total / points.size();
        }
    }

    // Tell all workers to start processing
    Ray_Trace rt;
    Ray r;
    rt.r = r; // dummy
    rt.t_min = -1;
    rt.t_max = -1;
    rt.ray_id = 0; // dummy
    std::cout << "Done sending Rays" << std::endl;
    for (int i = 1; i < np; ++i) {
        MPI_Ssend(&rt, sizeof(Ray_Trace), MPI_BYTE, i, 0, MPI_COMM_WORLD);
    }

    std::map<unsigned int, Ray_Hit_Remote> rhrs;

    int procs_to_recv = np - 1;
    while (procs_to_recv > 0) {
        MPI_Status status;
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == NOTHING_TO_SEND) {
            int dummy;
            MPI_Recv(&dummy, 1, MPI_INT, MPI_ANY_SOURCE, 
                        MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            procs_to_recv--;
            continue;
        } else {
            int byte_count;
            MPI_Get_count(&status, MPI_BYTE, &byte_count);
            int elements = byte_count / sizeof(Ray_Hit_Remote);
            Ray_Hit_Remote* rhr = new Ray_Hit_Remote[elements]; 
            MPI_Recv(rhr, byte_count, MPI_BYTE, status.MPI_SOURCE, 
                        status.MPI_TAG, MPI_COMM_WORLD, &status);

            for (int i = 0; i < elements; ++i) {
                std::pair<unsigned int, Ray_Hit_Remote> p;
                p.first = rhr[i].ray_id;
                p.second = rhr[i];
                auto res = rhrs.insert(p); 
                if (!res.second) { // already exists
                    if (res.first->second.t > rhr[i].t)
                        res.first->second = rhr[i];
                }
            }
            procs_to_recv--;
        }
    }

    int samples = points.size();
    for (auto& r : rhrs) {
        int x = r.first >> 19;
        int y = (r.first >> 6) & 0x00001FFF;
        image_buf[x][y] += r.second.col/ samples;
    }

    /*
    Ray r;
    Ray_Hit rh;
    Int_pair dest;
    int samples = sample_bins*sample_bins;
    omp_set_num_threads(1);
    #pragma omp parallel 
    {
    #pragma omp single private(r, dest, rh)
    {
    std::cout << "hi" << std::endl;
    while (!(local_rays.empty())) {
            r = local_rays.top();
            local_rays.pop();
            dest = local_dest.top();
            local_dest.pop();
        #pragma omp task firstprivate(r, dest, rh)
        {
        if (trace(r, EPSILON, FLT_MAX, rh, false)) {
            image_buf[dest.first][dest.second] += rh.col / samples;
        }
        }
    }
    }
    }
    */

    return;

}

void Ray_Tracer::write_buffer(std::string filename) {
    png::image< png::rgb_pixel > ray_image(x_res, y_res);
    for (unsigned int i = 0; i < x_res; ++i) {
        for (unsigned int j = 0; j < y_res; ++j) {
            ray_image[i][j] = image_buf[i][j].to_png_pixel();
        }
    }
    ray_image.write(filename);
}

static void gen_rand_samples(int bins_per_side, std::vector<V2>& points) {
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
