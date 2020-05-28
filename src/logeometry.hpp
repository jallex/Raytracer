#ifndef LOGEOMETRY_HPP_
#define LOGEOMETRY_HPP_

#include "./geometry.hpp"

//List of Geometry
class LoGeometry: public Geometry {
    public:
        LoGeometry(){}
        LoGeometry(Geometry **l, int n) {list = l, list_size = n;}
        virtual bool hit(const Ray& ray, float t_min, float t_max, hit_record& rec) const;
        Geometry **list;
        int list_size;
};

bool LoGeometry::hit(const Ray& ray, float t_min, float t_max, hit_record& rec) const{
    hit_record temp_rec;
    bool did_hit_something = false;
    //t of the closest hit (keep track of lowest t value)
    double closest_so_far = t_max;
    //iterate through each object in list
    for(int i = 0; i < list_size; i++) {
        //if the object at list[i] was hit
        if(list[i]->hit(ray, t_min, closest_so_far, temp_rec)) {
            did_hit_something = true;
            closest_so_far = temp_rec.t;
            //record the hit record of the closest object
            rec = temp_rec;
        }
    }
return did_hit_something;
}

#endif /* LOGEOMETRY_HPP_*/