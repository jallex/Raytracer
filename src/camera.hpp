#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "./rt_common.hpp"

class Camera {
    public:
    //constructor
    Camera() {
        //Camera intrinsics
        auto aspect_ratio = 16.0/9.0;
        //viewport height
        auto v_height = 2.0;
        //viewport width
        auto v_width = aspect_ratio * v_height;
        auto focal_length = 1.0;

        origin = Vector3(0, 0, 0);
        horizontal = Vector3(v_width, 0.0, 0.0);
        vertical = Vector3(0.0, v_height, 0.0);
        lower_left_corner = origin - horizontal/2 - vertical/2 - Vector3(0, 0, focal_length);
    }

    //add constructor for custom intrinsics

    Ray get_ray(float u, float v) const {
        return Ray(origin, lower_left_corner + horizontal*u + vertical*v - origin);
    }

    private:
    Vector3 origin;
    Vector3 lower_left_corner;
    Vector3 horizontal;
    Vector3 vertical;

};

#endif /* CAMERA_HPP_*/