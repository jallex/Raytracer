#include <iostream>
#include <fstream>
#include "./rt_common.hpp"
#include "./sphere.hpp"
#include "./LoGeometry.hpp"
using namespace std;

Vector3 color(const Ray& r, const Geometry& scene) {
    hit_record rec;
    if(scene.hit(r, 0.0, MAXFLOAT, rec)) {
        //calculate object color
        return ((rec.normal + Vector3(1, 1, 1)) * 0.5);
    }
    else {
        //calculate background color
        Vector3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.get_y() + 1.0);
        //When t=1.0 return blue. When t=0.0 return white depending on ray Y coordinate
        //linear interpolation form: blended_value = (1-t)*start_value + t*end_value where 0 <= t <= 1
        return Vector3(1.0, 1.0, 1.0)*(1.0 - t) + Vector3(0.5, 0.7, 1.0)*t;
    }
}

int main() {
    ofstream MyFile("my_image.ppm");
    //new
    const auto aspect_ratio = 16.0 / 9.0;
    //image width
    int width = 384;
    //image height
    int height = static_cast<int>(width / aspect_ratio);

    std::cout <<"P3\n" << width << " " << height << "\n255\n";
     MyFile <<"P3\n" << width << " " << height << "\n255\n";

    //viewport height
    auto v_height = 2.0;
    //viewport width
    auto v_width = aspect_ratio * v_height;
    auto focal_length = 1.0;

    auto origin = Vector3(0, 0, 0);
    auto horizontal = Vector3(v_width, 0, 0);
    auto vertical = Vector3(0, v_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - Vector3(0, 0, focal_length);
    
    //Create geometry
    LoGeometry scene;
    scene.add(make_shared<Sphere>(Vector3(0,0,-1), 0.5));
    scene.add(make_shared<Sphere>(Vector3(0,-100.5,-1), 100));

    for (int j = height - 1; j >= 0; j--){
         std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i < width; i++){
            auto u = float(i) / (width-1);
            auto v = float(j) / (height-1);
            Ray r(origin, (lower_left_corner + horizontal*u + vertical*v - origin));
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