#ifndef VEC3_HPP_
#define VEC3_HPP_

#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <vector>

namespace raytrace{

    class Vector3{
    private:
        float x_pos, y_pos, z_pos;
    public:
        Vector3() {}
        Vector3(float n):
            x_pos(n), y_pos(n), z_pos(n){}
        Vector3(float x, float y, float z):
            x_pos(x), y_pos(y), z_pos(z){}
        float get_x() const { return x_pos; }
        float get_y() const { return y_pos; }
        float get_z() const { return z_pos; }

        //Vector Operators 
        //overload these operators
        //multiply vector with a scalar, scalar must be on right hand side
        Vector3 operator * (const float &d) const {
            return Vector3(x_pos*d, y_pos*d, z_pos*d);
        }

        //multiply vector with another vector 
        Vector3 operator * (const Vector3 &v) const{
            //multiply this Vector's x, y, z, by input v's x, y, z
            return Vector3(x_pos*v.x_pos, y_pos*v.y_pos, z_pos*v.z_pos);
        }

        //divide vector with a scalar
        Vector3 operator / (const float &d) const {
            return Vector3(x_pos / d, y_pos / d, z_pos / d);
        }

        //divide vector with a vector
        Vector3 operator / (const Vector3&v) const {
            return Vector3(x_pos / v.x_pos, y_pos / v.y_pos, z_pos / v.z_pos);
        }

        //add a vector and a scalar
        Vector3 operator + (const float &d) const {
            return Vector3(x_pos + d, y_pos + d, z_pos + d);
        }

        //add a vector and a vector
        Vector3 operator + (const Vector3 &v) const {
            return Vector3(x_pos + v.x_pos, y_pos + v.y_pos, z_pos + v.z_pos);
        }

        //subtract a vector and a scalar
        Vector3 operator - (const float &d) const{
            return Vector3(x_pos - d, y_pos - d, z_pos - d);
        }

        //subtract a vector and a vector
        Vector3 operator - (const Vector3 &v) const{
            return Vector3(x_pos - v.x_pos, y_pos - v.y_pos, z_pos - v.z_pos);
        }

        //equivalent of a toString method
        //overload angle brackets, because of friend keyword able to use this outside of the vector class
        // when creating a Vector and use << to print our Vector
        //Now, we can print the Vector just using std::cout << v << endl;
        friend std::ostream & operator << (std::ostream &os, const Vector3 &v){ 
            //print out this instead of a memory address
            os << v.x_pos << " " << v.y_pos << " " << v.z_pos;
            return os;
        }

        //Dot product between two Vectors
        float dot_product(const Vector3 &v) const {
            return x_pos*v.x_pos + y_pos*v.y_pos + z_pos*v.z_pos;
        }

        //Cross product between two Vectors returns a Vector
        Vector3 cross_product(const Vector3 &v) const {
            return Vector3(y_pos * v.z_pos - z_pos*v.y_pos, 
            z_pos*v.x_pos - x_pos * v.z_pos,
            x_pos*v.y_pos - y_pos*v.x_pos);
        }

        //Magnitude
        float magnitude() const {
            return sqrt(pow(x_pos,2) + pow(y_pos,2) + pow(z_pos,2));
        }

        //Normalize
        //Use & keyword because we are passing in the reference to our object (changing current object, not making new vector)
        //Magnitude of normalized vector will be unit length, or 1
        Vector3 & normalize() {
            float inverse = 1/magnitude();
            //change each x, y, z to its inverse
            x_pos *= inverse;
            y_pos *= inverse;
            z_pos *= inverse;
            //return pointer to vector object
            //* is dereference operator, "this" is the pointer to the object
            return *this;
        }
    };
}

typedef raytrace::Vector3 Vector3;

inline Vector3 unit_vector(const Vector3 &v) {
    return v / v.magnitude();
}

#endif /* VEC3_HPP_ */

