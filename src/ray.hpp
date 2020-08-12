#ifndef RAY_HPP_
#define RAY_HPP_
#include "./vec3.hpp"

class Ray {
    public:
        Ray(){}
        Ray(const Vector3& vector1, const Vector3& vector2, float t = 0.0) 
        : originRay(vector1), directionRay(vector2), time(t){}

        Vector3 origin() const {
            return originRay;
        }
        Vector3 direction() const {
            return directionRay;
        }
        float getTime() const {
            return time;
        }

        //p(t) = A + t*B function that represents a Ray mathematically
        //Changing t gives you different points along the Ray
        Vector3 pointAtParameter(float t) const {
            return originRay + directionRay*t;
        }

    public:
        Vector3 originRay;
        Vector3 directionRay;
        float time;
};

#endif /* RAY_HPP_ */