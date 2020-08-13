#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include "./rtCommon.hpp"

class Camera {
    public:
    //constructor
    Camera(Vector3 lookfrom, Vector3 lookat, 
    Vector3 up, //view up : use world up (0,1,0) to specify up
    float fov, //vertical field-of-view in degrees
    float aspectRatio,
    float aperture, 
    float focusDist,   //Thin lens approxiation for depth of field 
    float t0 = 0,
    float t1 = 0
    ) {
        //Camera intrinsics
        auto thetaFov = degreesToRadians(fov);
        auto h = tan(thetaFov/2);

        //viewport height
        auto vHeight = 2.0 * h;
        //viewport width
        auto vWidth = aspectRatio * vHeight;

        //orthonormal basis (u, v, w) to describe camera’s orientation
        auto w = unitVector(lookfrom - lookat);
        auto u = unitVector(up.crossProduct(w));
        auto v = w.crossProduct(u);

        origin = lookfrom;
        horizontal = u * vWidth * focusDist;
        vertical = v * vHeight * focusDist;
        lowerLeftCorner = origin - horizontal/2 - vertical/2 - w*focusDist;
    
        lensRadius = aperture / 2;
        time0 = t0;
        time1 = t1;
    }

    Ray getRay(float s, float t) const {

        Vector3 rd = randomFromUnitLookfrom() * lensRadius;
        Vector3 offset = u * rd.getX() + v * rd.getY();
        
        return Ray(origin + offset, lowerLeftCorner + horizontal*s + vertical*t - origin - offset, randomNum(time0, time1)); 
}


    private:
    Vector3 origin;
    Vector3 lowerLeftCorner;
    Vector3 horizontal;
    Vector3 vertical;
    Vector3 u;
    Vector3 v;
    Vector3 w;
    float lensRadius;
    float time0; // camera's shutter open time
    float time1; // shutter close time
};

#endif /* CAMERA_HPP_*/