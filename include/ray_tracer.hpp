#ifndef RAY_TRACER_HPP
#define RAY_TRACER_HPP

#include <vector>
#include <string>

#include "camera.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "shape.hpp"

class Ray_Tracer {

    public:
        ~Ray_Tracer() { };
        Ray_Tracer(const Camera& cam, unsigned int x_res, unsigned int y_res,
                                        Color background_col, unsigned int sample_bins);

        void free_shapes();
        Color trace(const Ray& r, float t_min, float t_max, Ray_Hit& rh);
        void trace_all();
        void write_buffer(std::string filename);

        Camera cam;
        unsigned int x_res, y_res;
        Color background_col;
        unsigned int sample_bins;
        std::vector<Shape *> shapes;
        std::vector<std::vector<Color> > image_buf;
};

#endif
