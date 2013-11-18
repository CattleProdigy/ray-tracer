#include <Eigen/Core>
#include <png++/png.hpp>
#include "ray.hpp"

class Sphere {

    public:
        Sphere() {}; 
        ~Sphere() {};
        Sphere(float r, const Eigen::Vector3f& c);
        Sphere(const Sphere& s);
        Sphere& operator=(const Sphere& s);
        bool hit(const Ray& r, float t_min, float t_max); 

        Eigen::Vector3f c;
        float r; 

};
