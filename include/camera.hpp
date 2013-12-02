#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <Eigen/Core>

#include "ray.hpp"

using V3 = Eigen::Vector3f;

class Camera {
    public:
        Camera() {};
        Camera(const Camera& other);
        Camera(V3 eye, V3 gaze, V3 up, float s, float u0, float v0, float u1, float v1);

        Ray make_ray(float a, float b);

        V3 eye;                 // Camera Eyepoint (origin)
        V3 gaze;                // Camera Gaze (where it is looking)
        V3 up;                  // Camera Up (Which direction is up (for orientation)
        V3 u, v, w;             // Object space basis vectors
        V3 vp_corner;           // Lower left corner of viewing plane   
        V3 vp_up;               // Left edge of viewing plane
        V3 vp_across;           // Bottom edge of viewing plane
        float s;                // Distance between eye point and viewing plane
        float u0, v0, u1, v1;   // Lower left and upper right viewing plane corners
};

#endif
