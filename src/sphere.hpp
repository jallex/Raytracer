#ifndef SPHERE_HPP_
#define SPHERE_HPP_
#include "./geometry.hpp"

class Sphere: public Geometry {
    public:
    Sphere(){}
    Sphere(Vector3 c, float r) : center(c), radius(r) {};
    virtual bool hit(const Ray& ray, float tmin, float tmax, hit_record& rec) const;
    Vector3 center;
    float radius;
};

bool Sphere::hit(const Ray& ray, float tmin, float tmax, hit_record& rec) const {
    // return quadratic equation dot(B, B)*t^2 + 2*dot(B, A-C)*t + dot(A-C, A-C) - Radius*Radius = 0
    // where discriminant is b^2 - 4ac from form at^2 + bt + c = 0 
    Vector3 A = ray.origin();
    Vector3 B = ray.direction();
    Vector3 C = center;
    float a = B.dot_product(B);
    float b = (A - C).dot_product(B) * 2.0;
    float c = (A-C).dot_product(A-C) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    // D > 0 means two real, distinct roots; D = 0 means two real, identical roots; D < 0 means no real roots.
    if (discriminant > 0) {
        //did hit sphere
        //roots of the quadratic formula are found with quadratic equation that represents hit points
        //quadratic equation is: (-b+-√b^2-4ac) / 2a
        //Let’s assume the closest hit point (smallest t), so we only subtract the discriminant
        float root = (-b - sqrt(discriminant)) / (2.0*a);
        if (root < tmax && root > tmin) {
            rec.t = root;
            rec.p = ray.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}


#endif /* SPHERE_HPP_*/