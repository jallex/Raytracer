#ifndef MOVINGSPHERE_HPP_
#define MOVINGSPHERE_HPP_
 
#include "./logeometry.hpp"
#include "./rtCommon.hpp"
 
//Represents moving spheres
class MovingSphere : public Geometry {
   public:
   MovingSphere(){}
   MovingSphere(Vector3 c0, Vector3 c1, float t0, float t1, float r, shared_ptr<Material> m)
   :center0(c0), center1(c1), time0(t0), time1(t1), radius(r), mat_ptr(m){};
 
  virtual bool hit(const Ray& r, float tmin, float tmax, hitRecord& rec) const override;
  virtual bool boundingBox(float t0, float t1, AABB& outputBox) const override;

  Vector3 center(float time) const;
 
   public:
   Vector3 center0;
   Vector3 center1;
   float time0;
   float time1;
   float radius;
   shared_ptr<Material> mat_ptr;
};
 
Vector3 MovingSphere::center(float time) const {
   return center0 + (center1 - center0)*((time-time0) / (time1 - time0));
}

bool MovingSphere::hit(const Ray& r, float tmin, float tmax, hitRecord& rec) const{
    // return quadratic equation dot(B, B)*t^2 + 2*dot(B, A-C)*t + dot(A-C, A-C) - Radius*Radius = 0
    // where discriminant is b^2 - 4ac from form at^2 + bt + c = 0 
    Vector3 A = r.origin() - center(r.getTime());
    Vector3 B = r.direction();
    Vector3 C = center(r.getTime());
    float a = B.dotProduct(B);
    float b = (A - C).dotProduct(B) * 2.0;
    float c = (A-C).dotProduct(A-C) - radius * radius;
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
            rec.p = r.pointAtParameter(rec.t);
            //Add surface side determination 
            rec.normal = (rec.p - center(r.getTime())) / radius;
            rec.setFaceNormal(r, rec.normal);
            //record material of this sphere
            rec.matPtr = mat_ptr;
            return true;
        }
        //the second root based on quadratic equation
        root = (-b + sqrt(discriminant)) / (2.0*a);
        if (root < tmax && root > tmin) {
            rec.t = root;
            rec.p = r.pointAtParameter(rec.t);
            //Add surface side determination 
            rec.normal = (rec.p - center(r.getTime())) / radius;
            rec.setFaceNormal(r, rec.normal);
            //record material of this sphere
            rec.matPtr = mat_ptr;
            return true;
        }
    }
    return false;
}

bool MovingSphere::boundingBox(float t0, float t1, AABB& outputBox) const{
    AABB box0(center(t0) - Vector3(radius, radius, radius),
        center(t0) + Vector3(radius, radius, radius));
    AABB box1(center(t1) - Vector3(radius, radius, radius),
        center(t1) + Vector3(radius, radius, radius));
    outputBox = surroundingBox(box0, box1);

return true;
}

#endif /* MOVINGSPHERE_HPP_ */

