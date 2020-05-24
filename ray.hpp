#ifndef RAY_HPP_
#define RAY_HPP_
#include "vec3.hpp"

template <typename T>
class ray {
    public:
    ray(){}

    ray(const Vector3<T>& vector1, const Vector3<T>& vector2) 
    {
        Origin_Ray = vector1;
        Direction_Ray = vector2;
    }

    Vector3<T> origin() const {
        return Origin_Ray;
    }

    Vector3<T> direction() const {
        return Direction_Ray;
    }

    Vector3<T> point_at_parameter(float t) const {
        return Origin_Ray + Direction_Ray*t;
    }

Vector3<T> Origin_Ray;
Vector3<T> Direction_Ray;

};

#endif /* RAY_HPP_ */