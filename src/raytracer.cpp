#include <iostream>
#include <fstream>
#include "./ray.hpp"
using namespace std;

Vector3 color(const RayF r) {
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