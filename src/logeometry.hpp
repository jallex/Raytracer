#ifndef LOGEOMETRY_HPP_
#define LOGEOMETRY_HPP_

#include "./geometry.hpp"

#include <memory> //to include shared_ptr
#include <vector> //generic array-like collection

using std::shared_ptr;
using std::make_shared;

//List of Geometry
class LoGeometry: public Geometry {
    public:
        LoGeometry(){}
        //shared_ptr<type> : a pointer to some type, with reference-counting semantics
        //allows multiple geometries to share a common instance (for example, a bunch of spheres 
        //that all use the same texture map material), and makes memory management automatic 
        LoGeometry(shared_ptr<Geometry> object) { add(object); }
        //clear all objects from memory
        void clear() { objects.clear(); }
        //add object to end of this list of geometry
        void add(shared_ptr<Geometry> object) { objects.push_back(object); }
        //return if ray hit anything in this list of geometry
        virtual bool hit(const Ray& ray, float t_min, float t_max, hit_record& rec) const;
        //std::vector automatically grows as more values are added
        std::vector<shared_ptr<Geometry>> objects;
};

bool LoGeometry::hit(const Ray& ray, float t_min, float t_max, hit_record& rec) const{
    hit_record temp_rec;
    bool did_hit_something = false;
    //t of the closest hit (keep track of lowest t value)
    double closest_so_far = t_max;
    //iterate through each object in list
    for (const auto& object : objects) {
        //if the object at list[i] was hit
        if(object->hit(ray, t_min, closest_so_far, temp_rec)) {
            did_hit_something = true;
            closest_so_far = temp_rec.t;
            //record the hit record of the closest object
            rec = temp_rec;
        }
    }
return did_hit_something;
}

#endif /* LOGEOMETRY_HPP_*/