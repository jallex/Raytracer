//contain common constants and utility functions for this raytracer

#ifndef RT_COMMON_HPP_
#define RT_COMMON_HPP_

//Includes:
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings:
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants:
const float pi = 3.1415926535897932385;
const float infinity = std::numeric_limits<float>::infinity();

// Utility Functions:
inline float degreesToRadians(float degrees) {
    return degrees * pi / 180;
}

//return a random number 0 <= x < 1
inline float randomNum() {
    return rand() / (RAND_MAX + 1.0);
}

//return a random number min <= x < max
inline float randomNum(float min, float max) {
    return randomNum()*(max-min) + min;
}

//restrict color to the range [min, max]
inline float restrictColor(float color, float min, float max) {
 if (color < min) {
     return min;
 }
 else if (color > max) {
     return max;
 }
 else{
     return color;
 }
}


// Common Headers:
#include "ray.hpp"
#include "vec3.hpp"

#endif /* RT_COMMON_HPP_*/