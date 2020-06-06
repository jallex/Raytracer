#include <iostream>
#include <fstream>
#include "./rt_common.hpp"
#include "./sphere.hpp"
#include "./geometry.hpp"
#include "./logeometry.hpp"
#include "./camera.hpp"
#include "./color.hpp"
#include "./vec3.hpp"
#include "./material.hpp"
using namespace std;

Vector3 color(const Ray& r, const Geometry& scene, int depth) {
    hit_record rec;
    if(depth <= 0) { 
        //no light
        return Vector3(0, 0, 0);
    }
    if(scene.hit(r, 0.001, MAXFLOAT, rec)) {
        Ray scattered;
        Vector3 attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
            return attenuation * color(scattered, scene, depth-1);
        }
        return Vector3(0, 0, 0); //no light
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
    const int samples_per_pixel = 100;
    const int max_depth = 50;

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
    scene.add(make_shared<Sphere>(Vector3(0,0,-1), 0.5,
    make_shared<Lambertian>(Vector3(0.7, 0.3, 0.3))));
    scene.add(make_shared<Sphere>(Vector3(0,-100.5,-1), 100,
    make_shared<Lambertian>(Vector3(0.8, 0.8, 0.0))));

    scene.add(make_shared<Sphere>(Vector3(1,0,-1), 0.5, 
    make_shared<Metal>(Vector3(.8,.6,.2))));
    scene.add(make_shared<Sphere>(Vector3(-1,0,-1), 0.5, 
    make_shared<Metal>(Vector3(.8,.8,.8))));

    //Add camera to scene
    Camera cam;

    for (int j = height - 1; j >= 0; j--){
         std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i < width; i++){
            //color of this pixel
            Vector3 col(0,0,0);
            for(int s = 0; s < samples_per_pixel; s++) {
                //antialiasing, blur edges by generating pixels w multiple samples
                auto u = (i + random_num()) / (width - 1);
                auto v = (j + random_num()) / (height - 1);
                Ray r = cam.get_ray(u, v);
                col += color(r, scene, max_depth);
            }
            //print colors and write to ppm file
            write_color(std::cout, MyFile, col, samples_per_pixel);
        }
    }
     MyFile.close();
     std::cerr << "\nFinished!\n";
}