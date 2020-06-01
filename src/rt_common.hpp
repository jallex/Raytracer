//contain common constants and utility functions for this raytracer

#ifndef RT_COMMON_HPP_
#define RT_COMMON_HPP_

//Includes
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const float pi = 3.1415926535897932385;

// Utility Functions

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180;
}

// Common Headers
#include "ray.hpp"
#include "vec3.hpp"

#endif /* RT_COMMON_HPP_*/