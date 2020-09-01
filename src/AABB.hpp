#ifndef AABB_HPP_
#define AABB_HPP_

#include "./rtCommon.hpp"

//Axis aligned bounding box
class AABB {
    public:
    AABB(){}
    AABB(const Vector3& point1, const Vector3& point2) {
        _min = point1;
        _max = point2;
    }

    Vector3 min() const {
        return _min;
    }

    Vector3 max() const {
        return _max;
    }

    inline bool hit(const Ray& r, float tmin, float tmax) const {
        //Using the slab method:
        //compute (tx0, tx1)
        //compute (ty0, ty1)
        //compute (tz0, tz1)
        //return overlap?( (tx0, tx1), (ty0, ty1), (tz0, tz1))


        //X
        //𝑡𝑥0=min((𝑥0−𝐴𝑥)/𝑏𝑥,(𝑥1−𝐴𝑥)/𝑏𝑥)
        //𝑡𝑥1=max((𝑥0−𝐴𝑥)/𝑏𝑥,(𝑥1−𝐴𝑥)/𝑏𝑥)
        auto t0 = fmin((_min.getX() - r.origin().getX()) / r.direction().getX(),
                               (_max.getX() - r.origin().getX()) / r.direction().getX());
        auto t1 = fmax((_min.getX() - r.origin().getX()) / r.direction().getX(),
                               (_max.getX() - r.origin().getX()) / r.direction().getX());
        tmin = fmax(t0, tmin);
        tmax = fmin(t1, tmax);
        if (tmax <= tmin)
            return false;
        //Y
        t0 = fmin((_min.getY() - r.origin().getY()) / r.direction().getY(),
                               (_max.getY() - r.origin().getY()) / r.direction().getY());
        t1 = fmax((_min.getX() - r.origin().getY()) / r.direction().getY(),
                               (_max.getY() - r.origin().getY()) / r.direction().getY());
        tmin = fmax(t0, tmin);
        tmax = fmin(t1, tmax);
        if (tmax <= tmin)
            return false;
        //Z
        t0 = fmin((_min.getZ() - r.origin().getZ()) / r.direction().getZ(),
                               (_max.getZ() - r.origin().getZ()) / r.direction().getZ());
        t1 = fmax((_min.getX() - r.origin().getZ()) / r.direction().getZ(),
                               (_max.getZ() - r.origin().getZ()) / r.direction().getZ());
        tmin = fmax(t0, tmin);
        tmax = fmin(t1, tmax);
        if (tmax <= tmin)
            return false;

        return true;
    }

    //local variables members of this class
    Vector3 _max;
    Vector3 _min;

};

//create the bounding box comprised of 2 boxes
AABB surroundingBox(AABB box0, AABB box1){
    Vector3 small(fmin(box0.min().getX(), box1.min().getX()),
                fmin(box0.min().getY(), box1.min().getY()),
                fmin(box0.min().getZ(), box1.min().getZ()));

    Vector3 big(fmax(box0.max().getX(), box1.max().getX()),
            fmax(box0.max().getY(), box1.max().getY()),
            fmax(box0.max().getZ(), box1.max().getZ()));
    return AABB(small, big);
}

#endif /* AABB_HPP_*/