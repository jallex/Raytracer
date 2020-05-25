#ifndef RAY_HPP_
#define RAY_HPP_
#include "./vec3.hpp"

class Ray {
    public:
        Ray(){}

        Ray(const Vector3& vector1, const Vector3& vector2) 
        {
            Origin_Ray = vector1;
            Direction_Ray = vector2;
        }

        Vector3 origin() const {
            return Origin_Ray;
        }

        Vector3 direction() const {
            return Direction_Ray;
        }

        Vector3 point_at_parameter(float t) const {
            return Origin_Ray + Direction_Ray*t;
        }

Vector3 Origin_Ray;
Vector3 Direction_Ray;

};

typedef Ray RayF;

#endif /* RAY_HPP_ */