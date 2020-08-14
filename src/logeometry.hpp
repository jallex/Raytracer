#ifndef LOGEOMETRY_HPP_
#define LOGEOMETRY_HPP_

#include "./geometry.hpp"
#include "./AABB.hpp"

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
        virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
        virtual bool boundingBox(float t0, float t1, AABB& outputBox) const override;
        //std::vector automatically grows as more values are added
        std::vector<shared_ptr<Geometry>> objects;
};

bool LoGeometry::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const{
    hitRecord tempRec;
    bool didHitSomething = false;
    //t of the closest hit (keep track of lowest t value)
    double closestSoFar = tMax;
    //iterate through each object in list
    for (const auto& object : objects) {
        //if the object at list[i] was hit
        if(object->hit(ray, tMin, closestSoFar, tempRec)) {
            didHitSomething = true;
            closestSoFar = tempRec.t;
            //record the hit record of the closest object
            rec = tempRec;
        }
    }
return didHitSomething;
}

bool LoGeometry::boundingBox(float t0, float t1, AABB& outputBox) const{
    if(objects.empty()){
        return false;
    }
    AABB tempBox;
    bool box0 = true;

    for(const auto& object : objects){
        if(!object->boundingBox(t0, t1, tempBox)){
            return false;
        }
        outputBox = box0 ? tempBox : surroundingBox(outputBox, tempBox);
        box0 = false;
    }
    return true;
}

#endif /* LOGEOMETRY_HPP_*/