#include <iostream>
#include <fstream>
#include "./ray.hpp"
using namespace std;

//Does the ray intersect with a sphere at tghe given center and radius?ßß
bool did_hit_sphere(const Vector3 center, float radius, const RayF& ray) {
    // return quadratic equation dot(B, B)*t^2 + 2*dot(B, A-C)*t + dot(A-C, A-C) - R*R = 0
    // where discriminant is b^2 - 4ac from form at^2 + bt + c = 0 
    Vector3 A = ray.origin();
    Vector3 B = ray.direction();
    Vector3 C = center;
    float a = B.dot_product(B);
    float b = (A - C).dot_product(B) * 2.0;
    float c = (A-C).dot_product(A-C) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    // discriminant d < 0 means no real roots, d = 0 means 1 real root, d > 0 means 2 real roots.
    return (0 < discriminant);
}

Vector3 color(const RayF r) {
    if(did_hit_sphere(Vector3(0, 0, -1), 0.5, r)){
        return Vector3(1, 0, 0);
    }
    Vector3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.get_y() + 1.0);
    //linear interpolation form: blended_value = (1-t)*start_value + t*end_value where 0 <= t <= 1
    return Vector3(1.0, 1.0, 1.0)*(1.0 - t) + Vector3(0.5, 0.7, 1.0)*t;
}

int main(int argc, char** args) {
    ofstream MyFile("my_image.ppm");
    //new
    int nx = 200;
    int ny = 100;

    std::cout <<"P3\n" << nx << " " << ny << "\n255\n";
     MyFile <<"P3\n" << nx << " " << ny << "\n255\n";
    Vector3 lower_left_corner(-2.0, -1.0, -1.0);
    Vector3 horizontal(4.0, 0.0, 0.0);
    Vector3 vertical(0.0, 2.0, 0.0);
    Vector3 origin(0.0, 0.0, 0.0);
    for (int j = ny - 1; j >= 0; j--){
        for(int i = 0; i < nx; i++){
            float width = float(i) / float(nx);
            float height = float(j) / float(ny);
            RayF r(origin, (lower_left_corner + horizontal*width + vertical*height));
            
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