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
       auto invD = 1.0f / r.direction().getX();
       auto t0 = (min().getX() - r.origin().getX()) * invD;
        auto t1 = (max().getX() - r.origin().getX()) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin)
            return false;
        //Y
        invD = 1.0f / r.direction().getY();
        t0 = (min().getY() - r.origin().getY()) * invD;
        t1 = (max().getY() - r.origin().getY()) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin)
            return false;
        //Z
        invD = 1.0f / r.direction().getZ();
        t0 = (min().getZ() - r.origin().getZ()) * invD;
        t1 = (max().getZ() - r.origin().getZ()) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
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