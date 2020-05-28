#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_
#include "./ray.hpp"

struct hit_record {
    //t of the hit 
    float t;
    //the point of hit
    Vector3 p;
    //normal
    Vector3 normal;
};

//Geometry represents either a single piece of geometry like a sphere, or a list of multiple geometry.
class Geometry {
    public:
    //virtual function ensures we always override the function 
    virtual bool hit(const Ray& ray, float t_min, float t_max, hit_record& rec) const=0;
};


#endif /* GEOMETRY_HPP_*/