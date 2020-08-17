#ifndef GEOMETRY_HPP_
#define GEOMETRY_HPP_
#include "./ray.hpp"
#include "./AABB.hpp"

class Material; //alert compiler that pointer is to a class

//contains necessary arguments and info
struct hitRecord {
    //t of the hit 
    float t;
    //the point of hit
    Vector3 p;
    //U surface coordinate of ray-object hit point
    float u; 
    //V surface coordinate of ray-object hit point
    float v; 
    //normal
    Vector3 normal;
    //Materials
    shared_ptr<Material> matPtr;

    bool frontFace;

    //set up normals to always point “outward” from the surface
    inline void setFaceNormal(const Ray& r, const Vector3& outwardNormal) {
        frontFace = r.direction().dotProduct(outwardNormal) < 0;
        //if frontFace < 0, then normal = outwardNormal, else it is -outwardNormal
        normal = frontFace ? outwardNormal :outwardNormal*(-1);
    }
};

//Geometry represents either a single piece of geometry like a sphere, or a list of multiple geometry.
class Geometry {
    public:
    //virtual function ensures we always override the function 
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const=0;
    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const = 0;
};


#endif /* GEOMETRY_HPP_*/