#include <iostream>
#include <fstream>
#include "./sphere.hpp"
#include "./LoGeometry.hpp"
using namespace std;

Vector3 color(const Ray& r, Geometry *scene) {
    hit_record rec;
    if(scene->hit(r, 0.0, MAXFLOAT, rec)) {
        //calculate object color
        return Vector3(rec.normal.get_x() + 1, rec.normal.get_y() + 1, rec.normal.get_z() + 1)*0.5;
    }
    else {
        //calculate background color
        Vector3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.get_y() + 1.0);
        //linear interpolation form: blended_value = (1-t)*start_value + t*end_value where 0 <= t <= 1
        return Vector3(1.0, 1.0, 1.0)*(1.0 - t) + Vector3(0.5, 0.7, 1.0)*t;
    }
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
    Geometry *list[2];
    list[0] = new Sphere(Vector3(0,0,-1), 0.5);
    list[1] = new Sphere(Vector3(0,-100.5, -1), 100);
    Geometry *scene = new LoGeometry(list, 2);
    for (int j = height - 1; j >= 0; j--){
        for(int i = 0; i < width; i++){
            float x_pos = float(i) / float(width);
            float y_pos = float(j) / float(height);
            Ray r(origin, (lower_left_corner + horizontal*x_pos + vertical*y_pos));
            Vector3 p = r.point_at_parameter(2.0);
            Vector3 col = color(r, scene);
            int ir = int(255.99*col.get_x());
            int ig = int(255.99*col.get_y());
            int ib = int(255.99*col.get_z());

            std::cout << ir << " " << ig << " " << ib << "\n";
            MyFile << ir << " " << ig << " " << ib << "\n";
        }
    }
     MyFile.close();
}