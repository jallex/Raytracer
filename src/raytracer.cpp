#include <iostream>
#include <fstream>
#include "./ray.hpp"
using namespace std;

//returns where the ray hit the sphere
float hit_sphere_at(const Vector3 center, float radius, const RayF& ray) {
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
    if (discriminant < 0) {
        return -1.0; //did not hit sphere
    }
    else {
        //roots of the quadratic formula are found with quadratic equation that represents hit points
        //quadratic equation is: (-b+-√b^2-4ac) / 2a
        //Let’s assume the closest hit point (smallest t), so we only subtract the discriminant
        return (-b - sqrt(discriminant)) / (2.0*a);
    }
}

Vector3 color(const RayF r) {
    Vector3 sphere_center = Vector3(0, 0, -1);
    float sphere_radius = 0.3;
    float t = (hit_sphere_at(sphere_center, sphere_radius, r));
    if (t > 0.0) {
        //make the normal unit length vector– so each component is between -1 and 1
        //map each component to the interval from 0 to 1, and then map x/y/z to r/g/b
        Vector3 surface_normal = unit_vector(r.point_at_parameter(t) - sphere_center);
        return Vector3(surface_normal.get_x() + 1, surface_normal.get_y() + 1, surface_normal.get_z() + 1) * 0.5;
    }
    Vector3 unit_direction = unit_vector(r.direction());
    t = 0.5*(unit_direction.get_y() + 1.0);
    //linear interpolation form: blended_value = (1-t)*start_value + t*end_value where 0 <= t <= 1
    return Vector3(1.0, 1.0, 1.0)*(1.0 - t) + Vector3(0.5, 0.7, 1.0)*t;
}

int main(int argc, char** args) {
    ofstream MyFile("my_image.ppm");
    //new
    int width = 200;
    int height = 100;

    std::cout <<"P3\n" << width << " " << height << "\n255\n";
     MyFile <<"P3\n" << width << " " << height << "\n255\n";
    Vector3 lower_left_corner(-2.0, -1.0, -1.0);
    Vector3 horizontal(4.0, 0.0, 0.0);
    Vector3 vertical(0.0, 2.0, 0.0);
    Vector3 origin(0.0, 0.0, 0.0);
    for (int j = height - 1; j >= 0; j--){
        for(int i = 0; i < width; i++){
            float x_pos = float(i) / float(width);
            float y_pos = float(j) / float(height);
            RayF r(origin, (lower_left_corner + horizontal*x_pos + vertical*y_pos));
            
            Vector3 col = color(r);
            int ir = int(255.99*col.get_x());
            int ig = int(255.99*col.get_y());
            int ib = int(255.99*col.get_z());

            std::cout << ir << " " << ig << " " << ib << "\n";
            MyFile << ir << " " << ig << " " << ib << "\n";
        }
    }
     MyFile.close();
}