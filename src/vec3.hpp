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
#include "./rtCommon.hpp"

namespace raytrace{

    class Vector3{
    private:
        float xPos, yPos, zPos;
    public:
        Vector3() {}
        Vector3(float n):
            xPos(n), yPos(n), zPos(n){}
        Vector3(float x, float y, float z):
            xPos(x), yPos(y), zPos(z){}
        float getX() const { return xPos; }
        float getY() const { return yPos; }
        float getZ() const { return zPos; }

        float vecLengthSquared() const {
            return xPos*xPos + yPos*yPos + zPos*zPos;
        }

        //Vector Operators 
        //overload these operators
        //multiply vector with a scalar, scalar must be on right hand side
        Vector3 operator * (const float &d) const {
            return Vector3(xPos*d, yPos*d, zPos*d);
        }

        //multiply vector with another vector 
        Vector3 operator * (const Vector3 &v) const{
            //multiply this Vector's x, y, z, by input v's x, y, z
            return Vector3(xPos*v.xPos, yPos*v.yPos, zPos*v.zPos);
        }

        //divide vector with a scalar
        Vector3 operator / (const float &d) const {
            return Vector3(xPos / d, yPos / d, zPos / d);
        }

        //divide vector with a vector
        Vector3 operator / (const Vector3&v) const {
            return Vector3(xPos / v.xPos, yPos / v.yPos, zPos / v.zPos);
        }

        //add a vector and a scalar
        Vector3 operator + (const float &d) const {
            return Vector3(xPos + d, yPos + d, zPos + d);
        }

        //add a vector and a vector
        Vector3 operator + (const Vector3 &v) const {
            return Vector3(xPos + v.xPos, yPos + v.yPos, zPos + v.zPos);
        }

        //subtract a vector and a scalar
        Vector3 operator - (const float &d) const{
            return Vector3(xPos - d, yPos - d, zPos - d);
        }

        //subtract a vector and a vector
        Vector3 operator - (const Vector3 &v) const{
            return Vector3(xPos - v.xPos, yPos - v.yPos, zPos - v.zPos);
        }

        //equivalent of a toString method
        //overload angle brackets, because of friend keyword able to use this outside of the vector class
        // when creating a Vector and use << to print our Vector
        //Now, we can print the Vector just using std::cout << v << endl;
        friend std::ostream & operator << (std::ostream &os, const Vector3 &v){ 
            //print out this instead of a memory address
            os << v.xPos << " " << v.yPos << " " << v.zPos;
            return os;
        }

        //Dot product between two Vectors
        float dotProduct(const Vector3 &v) const {
            return xPos*v.xPos + yPos*v.yPos + zPos*v.zPos;
        }

        //Cross product between two Vectors returns a Vector
        Vector3 crossProduct(const Vector3 &v) const {
            return Vector3(yPos * v.zPos - zPos*v.yPos, 
            zPos*v.xPos - xPos * v.zPos,
            xPos*v.yPos - yPos*v.xPos);
        }

        //Magnitude
        float magnitude() const {
            return sqrt(pow(xPos,2) + pow(yPos,2) + pow(zPos,2));
        }

        //Normalize
        //Use & keyword because we are passing in the reference to our object (changing current object, not making new vector)
        //Magnitude of normalized vector will be unit length, or 1
        Vector3 & normalize() {
            float inverse = 1/magnitude();
            //change each x, y, z to its inverse
            xPos *= inverse;
            yPos *= inverse;
            zPos *= inverse;
            //return pointer to vector object
            //* is dereference operator, "this" is the pointer to the object
            return *this;
        }

        //*= a vector and a vector
        Vector3 & operator *= (const Vector3 &v) {
            xPos *= v.xPos;
            yPos *= v.yPos;
            zPos *= v.zPos;
            return *this;
        }

        //*= a vector and a float
        Vector3 & operator *= (const float &d) {
            xPos *= d;
            yPos *= d;
            zPos *= d;
            return *this;
        }

        //+= a vector and a vector
        Vector3 & operator += (const Vector3 &v) {
            xPos += v.xPos;
            yPos += v.yPos;
            zPos += v.zPos;
            return *this;
        }

        // /= a vector and a vector
        Vector3 & operator /= (const Vector3 &v) {
            xPos /= v.xPos;
            yPos /= v.yPos;
            zPos /= v.zPos;
            return *this;
        }

        // /= a vector and a float
        Vector3 & operator /= (const float &d) {
            xPos /= d;
            yPos /= d;
            zPos /= d;
            return *this;
        }

        // -= a vector and a vector
        Vector3 & operator -= (const Vector3 &v) {
            xPos -= v.xPos;
            yPos -= v.yPos;
            zPos -= v.zPos;
            return *this;
        }
    };
}

typedef raytrace::Vector3 Vector3;

inline static Vector3 randomVec(){
    return Vector3(randomNum(), randomNum(), randomNum());
}

inline static Vector3 randomVec(float min, float max){
    return Vector3(randomNum(min, max), randomNum(min, max), randomNum(min, max));
}

//Material functions to calculate how rays interact with different materials

//Diffuse  method 1
Vector3 randomInUnitSphere() {
    while (true) {
        auto p = randomVec(-1,1);
        if (p.vecLengthSquared() >= 1) continue;
        return p;
    }
}

//Diffuse method 2
//generate a random unit vector with lambertian distribution 
inline Vector3 randomUnitVec() {
    //random num between 0 and diameter
    auto a = randomNum(0, 2*pi);
    //random between -1 to 1
    auto z = randomNum(-1, 1);
    auto r = sqrt(1-z*z);
    return Vector3(r*cos(a), r*sin(a), z);
}

inline Vector3 unitVector(const Vector3 &v) {
    return v / v.magnitude();
}

//Diffuse method 3
Vector3 randomInHemisphere(const Vector3& normal) {
    Vector3 inUnitSphere = randomInUnitSphere();
    if (inUnitSphere.dotProduct(normal) > 0.0) // In the same hemisphere as the normal
        return inUnitSphere;
    else
        return inUnitSphere*(-1);
}

//Reflected ray = v + 2b
Vector3 reflect(const Vector3& v, const Vector3& normal) {
    //length of b is the dot product of v and n
    float lengthB = v.dotProduct(normal);
    //subtract bc v points in
    return v - normal*lengthB*2.0;
}

//Refraction
Vector3 refract(const Vector3& uv, const Vector3& normal, float etaIOverEtaT) {
    auto cosTheta = (uv*(-1)).dotProduct(normal);
    Vector3 rParallel = (uv + normal*cosTheta)*etaIOverEtaT;
    Vector3 rPerp = normal*sqrt(1.0 - rParallel.vecLengthSquared())*(-1);
    return rParallel + rPerp;
}

//generate random scene rays originating from the lookfrom point
//the larger the radius, the greater the defocus blur
Vector3 randomFromUnitLookfrom(){
    while(true) {
        auto p = Vector3(randomNum(-1, 1), randomNum(-1, 1), 0);
        if(p.vecLengthSquared() >= 1) continue;
        return p;
    }
}

#endif /* VEC3_HPP_ */

