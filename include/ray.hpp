#ifndef RAY_HPP
#define RAY_HPP

#include <iostream>
#include <Eigen/Core>


class Ray {

    public: 
        
        // ctors
        Ray() {};
        Ray(const Ray& r);
        Ray(Eigen::Vector3f offset, Eigen::Vector3f slope);

        // dtor
        ~Ray() {};

        // Assignment
        Ray& operator=(const Ray& r);

        // Print
        friend std::ostream& operator<<(std::ostream& os, const Ray& r);
         
        Eigen::Vector3f o; // Offset
        Eigen::Vector3f s; // Slope
};

#endif
