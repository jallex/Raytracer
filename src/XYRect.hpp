#ifndef XYRECT_HPP_
#define XYRECT_HPP_
#include "./rtCommon.hpp"
#include "./geometry.hpp"

//axis aligned rectangle on the xy plane
class XYRect : public Geometry {
    public: 
    XYRect(){}

    XYRect(float _x0, float _x1, float _y0, float _y1, float _k, shared_ptr<Material> mat) 
    :x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat){};

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const override;

    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const override {
        //BB must have x, y, and z be > 0, so pad the z dimension a small amount
        outputBox = AABB(Vector3(x0, y0, k-0.0001), Vector3(x1, y1, k+0.0001));
        return true;
    }

    public:
    shared_ptr<Material> mp;
    float x0, x1, y0, y1, k;
};

bool XYRect::hit(const Ray& r, float t0, float t1, hitRecord& rec) const {
    auto t = (k-r.origin().getZ()) / r.direction().getZ();
    if (t < t0 || t > t1) {
        return false;
    }
    auto x = r.origin().getX() + t*r.direction().getX();
    auto y = r.origin().getY() + t*r.direction().getY();
    if(x < x0 || x > x1 || y < y0 || y > y1) {
        return false;
    }
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    auto outwardNormal = Vector3(0, 0, 1);
    rec.setFaceNormal(r, outwardNormal);
    rec.matPtr = mp;
    rec.p = r.pointAtParameter(t);
    return true;
}

//axis aligned rectangle on the XZ plane 
class XZRect : public Geometry {
    public: 
    XZRect(){}

    XZRect(float _x0, float _x1, float _z0, float _z1, float _k, shared_ptr<Material> mat) 
    :x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat){};

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const override;

    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const override {
        //BB must have x, y, and z be > 0, so pad the y dimension a small amount
        outputBox = AABB(Vector3(x0, k-0.0001, z0), Vector3(x1, k+0.0001, z1));
        return true;
    }

    public:
    shared_ptr<Material> mp;
    float x0, x1, z0, z1, k;
};

bool XZRect::hit(const Ray& r, float t0, float t1, hitRecord& rec) const {
    auto t = (k-r.origin().getY()) / r.direction().getY();
    if (t < t0 || t > t1) {
        return false;
    }
    auto x = r.origin().getX() + t*r.direction().getX();
    auto z = r.origin().getZ() + t*r.direction().getZ();
    if(x < x0 || x > x1 || z < z0 || z > z1) {
        return false;
    }
    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outwardNormal = Vector3(0, 1, 0);
    rec.setFaceNormal(r, outwardNormal);
    rec.matPtr = mp;
    rec.p = r.pointAtParameter(t);
    return true;
}

//axis aligned rectangle on the XZ plane 
class ZYRect : public Geometry {
    public: 
    ZYRect(){}

    ZYRect(float _y0, float _y1, float _z0, float _z1, float _k, shared_ptr<Material> mat) 
    :y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat){};

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const override;

    virtual bool boundingBox(float t0, float t1, AABB& outputBox) const override {
        //BB must have x, y, and z be > 0, so pad the x dimension a small amount
        outputBox = AABB(Vector3(k-0.0001, y0, z0), Vector3(k+0.0001, y1, z1));
        return true;
    }

    public:
    shared_ptr<Material> mp;
    float y0, y1, z0, z1, k;
};

bool ZYRect::hit(const Ray& r, float t0, float t1, hitRecord& rec) const {
    auto t = (k-r.origin().getX()) / r.direction().getX();
    if (t < t0 || t > t1) {
        return false;
    }
    auto y = r.origin().getY() + t*r.direction().getY();
    auto z = r.origin().getZ() + t*r.direction().getZ();
    if(y < y0 || y > y1 || z < z0 || z > z1) {
        return false;
    }
    rec.u = (y-y0)/(y1-y0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    auto outwardNormal = Vector3(1, 0, 0);
    rec.setFaceNormal(r, outwardNormal);
    rec.matPtr = mp;
    rec.p = r.pointAtParameter(t);
    return true;
}


#endif /* XYRECT_HPP_ */