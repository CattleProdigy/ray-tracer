#ifndef RAY_HPP
#define RAY_HPP

#include <iostream>
#include <Eigen/Core>

using V3 = Eigen::Vector3f;

class Ray {

    public: 
        
        // ctors
        Ray() {};
        Ray(const Ray& r);
        Ray(V3 offset,V3 slope, unsigned short depth);

        // dtor
        ~Ray() {};

        // Assignment
        Ray& operator=(const Ray& r);

        // Print
        friend std::ostream& operator<<(std::ostream& os, const Ray& r);

        // Evaluate
        V3 at(float t) const;

        V3 o; // Offset
        V3 s; // Slope
        unsigned short depth;
};

struct {
    Ray r;
    float t_min;
    float t_max;
    bool shadow;

} Ray_Trace;

#endif
