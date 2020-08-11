#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "./rt_common.hpp"

class Camera {
    public:
    //constructor
    Camera(Vector3 lookfrom, Vector3 lookat, 
    Vector3 up, //view up : use world up (0,1,0) to specify up
    float fov, //vertical field-of-view in degrees
    float aspect_ratio,
    float aperture, 
    float focus_dist   //Thin lens approxiation for depth of field 
    ) {
        //Camera intrinsics
        auto theta = degrees_to_radians(fov);
        auto h = tan(theta/2.0f);

        //viewport height
        auto v_height = 2.0 * h;
        //viewport width
        auto v_width = aspect_ratio * v_height;

        //orthonormal basis (u, v, w) to describe camera’s orientation
        auto w = unit_vector(lookfrom - lookat);
        auto u = unit_vector(up.cross_product(w));
        auto v = w.cross_product(u);

        origin = lookfrom;
        horizontal = u * v_width * focus_dist;
        vertical = v * v_height * focus_dist;
        lower_left_corner = origin - horizontal/2 - vertical/2 - w*focus_dist;
    
        lens_radius = aperture / 2;
    }

    Ray get_ray(float s, float t) const {

        Vector3 rd = random_from_unit_lookfrom() * lens_radius;
        Vector3 offset = u * rd.get_x() + v * rd.get_y();
        
        return Ray(origin + offset, lower_left_corner + horizontal*s + vertical*t - origin - offset); 
}


    private:
    Vector3 origin;
    Vector3 lower_left_corner;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 u;
    Vector3 v;
    Vector3 w;
    float lens_radius;
};

#endif /* CAMERA_HPP_*/