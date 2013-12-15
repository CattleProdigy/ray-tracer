#include <Eigen/Core>
#include <Eigen/Geometry>
#include <iostream>
#include <cstdio>

#include "camera.hpp"

using V3 = Eigen::Vector3f;

Camera::Camera(const Camera& other) {
    eye = other.eye;
    gaze = other.gaze;
    up = other.up;
    s = other.s;
    u = other.u;
    v = other.v;
    w = other.w;
    u0 = other.u0;
    v0 = other.v0;
    u1 = other.u1;
    v1 = other.v1;
}

Camera::Camera(V3 eye, V3 gaze, V3 up, float s, float u0, float v0, float u1, float v1) {
    this->eye  = eye;
    this->gaze = gaze;
    this->up   = up;
    this->s    = s;
    this->u0   = u0;
    this->v0   = v0;
    this->u1   = u1;
    this->v1   = v1;

    // Generate orthonormal basis for viewing plain
    w = -gaze.normalized();
    u = up.cross(w);
    u.normalize();
    v = w.cross(u);
   
    vp_across = (u1 - u0) * u;
    vp_up = (v1 - v0) * v;
    vp_corner = eye + u0*u + v0*v - s*w;
}

Ray Camera::make_ray(float a, float b) {
    V3 dir = (vp_corner + a*vp_up + b*vp_across) - eye; 
    dir.normalize();
    return Ray(eye, dir, 1);
}
