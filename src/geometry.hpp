#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_
#include "./ray.hpp"

class Material; //alert compiler that pointer is to a class

//contains necessary arguments and info
struct hit_record {
    //t of the hit 
    float t;
    //the point of hit
    Vector3 p;
    //normal
    Vector3 normal;
    //Materials
    shared_ptr<Material> mat_ptr;

    bool front_face;

    //set up normals to always point “outward” from the surface
    inline void set_face_normal(const Ray& r, const Vector3& outward_normal) {
        front_face = r.direction().dot_product(outward_normal) < 0;
        //if front_face < 0, then normal = outward_normal, else it is -outward_normal
        normal = front_face ? outward_normal :outward_normal*(-1);
    }
};

//Geometry represents either a single piece of geometry like a sphere, or a list of multiple geometry.
class Geometry {
    public:
    //virtual function ensures we always override the function 
    virtual bool hit(const Ray& ray, float t_min, float t_max, hit_record& rec) const=0;
};


#endif /* GEOMETRY_HPP_*/