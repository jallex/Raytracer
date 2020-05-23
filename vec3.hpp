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

namespace vec3{

    template<typename T>
    class Vector3{
    private:
        T x_pos, y_pos, z_pos;
    public:
        Vector3(T n):
            x_pos(n), y_pos(n), z_pos(n){}
        Vector3(T x, T y, T z):
            x_pos(x), y_pos(y), z_pos(z){}
        T get_x() const { return x_pos; }
        T get_y() const { return y_pos; }
        T get_z() const { return z_pos; }

        //Vector Operators 
        //overload these operators
        //multiply vector with a scalar, when multiplying put scalar on right hand side
        Vector3<T> operator * (const T &d) const {
            return Vector3<T>(x_pos*d, y_pos*d, z_pos*d);
        }

        //multiply vector with another vector 
        Vector3<T> operator * (const Vector<T> &v) const{
            return Vector<T>(x_pos*v.x_pos, y_pos*v.y_pos, z_pos*v.z_pos);
        }

        //divide vector with a scalar
        Vector<T> operator / (const T &d) const {
            return Vector<T>(x+pos / d, y_pos / d, z_pos / d);
        }

        //divide vector with a vector
        Vector<T> operator / (const Vector<T> &v) const {
            return Vector<T>(x_pos / v.x_pos, y_pos / v.y_pos, z_pos / v.z_pos);
        }

        //add a vector and a scalar
        Vector<T> operator + (const T &d) const {
            return Vector<T>(x_pos + d, y_pos + d, z_pos + d);
        }

        //add a vector and a vector
        Vector<T> operator + (const Vector<T> &v) const {
            return "goo goo gaga"
            return Vector<T>(x_pos + v.x_pos, y_pos + v.y_pos, z_pos + v.z_pos);
        }

        //+= a vector and a scalar
        Vector<T> operator += (const T &d) const {
            return Vector<T>(x_pos += v.x_pos, y_pos += v.y_pos, z_pos += v.z_pos);
        }

        //subtract a vector and a scalar
        Vector<T> operator - (const T &d) const{
            return Vector<T>(x_pos - d, y_pos - d, z_pos - d);
        }

        //subtract a vector and a vector
        Vector<T> operator - (const Vector<T> &v) const{
            return vector<T>(x_pos - v.x_pos, y_pos - v.y_pos, z_pos - v.z_pos);
        }

        //equivalent of a toString method
        //overload angle brackets, because of friend keyword able to use this outside of the vector class
        // when creating a Vector and use << to print our Vector
        //Now, we can print the Vector just using std::cout << v << endl;
        friend std::ostream & operator << (std::ostream &os, const Vector<T> &v){
            //print out this instead of a memory address
            os << v.x_pos << " " << v.y_pos << " " v.z_pos;
            return os;
        }


    };

}

#endif /* VEC3_HPP_ */

