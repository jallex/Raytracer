#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "./rt_common.hpp"

class Camera {
    public:
    //constructor
    Camera(Vector3 lookfrom, Vector3 lookat, 
    Vector3 up, //view up : use world up (0,1,0) to specify up
    float fov, //vertical field-of-view in degrees
    float aspect_ratio) {
        //Camera intrinsics
        auto theta = degrees_to_radians(fov);
        auto h = tan(theta/2);

        //viewport height
        auto v_height = 2.0 * h;
        //viewport width
        auto v_width = aspect_ratio * v_height;

        //orthonormal basis (u, v, w) to describe camera’s orientation
        auto w = unit_vector(lookfrom - lookat);
        auto u = unit_vector(up.cross_product(w));
        auto v = w.cross_product(u);

        origin = lookfrom;
        horizontal = u * v_width;
        vertical = v * v_height;
        lower_left_corner = origin - horizontal/2 - vertical/2 - w;
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