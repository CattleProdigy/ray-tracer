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


void Ray_Tracer::trace_all(int rank, int np) {

    std::vector<V2> points;
    Color total;
    Ray r;
    Ray_Hit rh;
    #pragma omp parallel for private(total, r, dest, rh)
    {
    for (unsigned int i = 0; i < x_res; ++i) {
        #pragma omp parallel for 
        for (unsigned int j = 0; j < y_res; ++j) {
            points.clear();
            gen_rand_samples(sample_bins, points);

            for (unsigned int k = 0; k < points.size(); ++k) {
                V2& p = points[k];
                r = cam.make_ray((i + p.x() + 0.5)/x_res, (j + p.y() + 0.5)/y_res);
                trace(r, EPSILON, FLT_MAX, rh, false);
                    total += rh.col;
            }
            image_buf[i][j] = total / points.size();
        }
    }
    }

    // Tell all workers to start processing
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
