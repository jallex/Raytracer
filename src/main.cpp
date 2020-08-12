#include <iostream>
#include <fstream>
#include "./rtCommon.hpp"
#include "./sphere.hpp"
#include "./geometry.hpp"
#include "./logeometry.hpp"
#include "./camera.hpp"
#include "./color.hpp"
#include "./vec3.hpp"
#include "./material.hpp"
using namespace std;

Vector3 color(const Ray& r, const Geometry& scene, int depth) {
    hitRecord rec;
    if(depth <= 0) { 
        //no light
        return Vector3(0, 0, 0);
    }
    if(scene.hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        Vector3 attenuation;
        if (rec.matPtr->scatter(r, rec, attenuation, scattered)){
            return attenuation * color(scattered, scene, depth-1);
        }
        return Vector3(0, 0, 0); //no light
    }
    else {
        //calculate background color 
        Vector3 unitDirection = unitVector(r.direction());
        auto t = 0.5*(unitDirection.getY() + 1.0);
        //When t=1.0 return blue. When t=0.0 return white depending on ray Y coordinate
        //linear interpolation form: blended_value = (1-t)*start_value + t*end_value where 0 <= t <= 1
        return Vector3(1.0, 1.0, 1.0)*(1.0 - t) + Vector3(0.5, 0.7, 1.0)*t;
    }
}

LoGeometry randomScene() {
    LoGeometry world;

    auto groundMaterial = make_shared<Lambertian>(Vector3(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Vector3(0,-1000,0), 1000, groundMaterial));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto chooseMat = randomNum();
            Vector3 center(a + 0.9*randomNum(), 0.2, b + 0.9*randomNum());

            if ((center - Vector3(4, 0.2, 0)).magnitude() > 0.9) {
                shared_ptr<Material> sphereMaterial;

                if (chooseMat < 0.8) {
                    // diffuse
                    auto albedo = randomVec() * randomVec();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                } else if (chooseMat < 0.95) {
                    // metal
                    auto albedo = randomVec(0.5, 1);
                    auto fuzz = randomNum(0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                } else {
                    // glass
                    sphereMaterial = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Vector3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Vector3(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Vector3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Vector3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Vector3(4, 1, 0), 1.0, material3));

    return world;
}

int main() {
    ofstream MyFile("myImage4.ppm");
    //new
    const auto aspectRatio = 16.0 / 9.0;
    //image width
    int width = 1200;
    //image height
    int height = static_cast<int>(width / aspectRatio);
    const int samplesPerPixel = 10;
    const int maxDepth = 50;

    std::cout <<"P3\n" << width << " " << height << "\n255\n";
     MyFile <<"P3\n" << width << " " << height << "\n255\n";

    //Create geometry
    //LoGeometry scene;

    //scene.add(make_shared<Sphere>(
        //Vector3(0,0,-1), 0.5, make_shared<Lambertian>(Vector3(0.1, 0.2, 0.5))));

    //scene.add(make_shared<Sphere>(
        //Vector3(0,-100.5,-1), 100, make_shared<Lambertian>(Vector3(0.8, 0.8, 0.0))));

    //scene.add(make_shared<Sphere>(Vector3(1,0,-1), 0.5, make_shared<Metal>(Vector3(.8, .6, .2), 0.0)));
    //scene.add(make_shared<Sphere>(Vector3(-1,0,-1), 0.5, make_shared<Dielectric>(1.5)));
    //creating a dielectric sphere with a negative radius makes surface normal point inwards,
    //creating a hollow glass sphere
    // scene.add(make_shared<Sphere>(Vector3(-1,0,-1), -0.45, make_shared<Dielectric>(1.5)));

    auto scene = randomScene();


    //Add camera to scene
    Vector3 lookfrom(13,2,3);
    Vector3 lookat(0,0,0);
    Vector3 vup(0,1,0);
    auto distToFocus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspectRatio, aperture, distToFocus);


/**
    Vector3 lookfrom(13,2,3);
    Vector3 lookat(0,0,0);
    Vector3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
**/

    for (int j = height - 1; j >= 0; j--){
         std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for(int i = 0; i < width; i++){
            //color of this pixel
            Vector3 col(0,0,0);
            for(int s = 0; s < samplesPerPixel; s++) {
                //antialiasing, blur edges by generating pixels w multiple samples
                auto u = (i + randomNum()) / (width - 1);
                auto v = (j + randomNum()) / (height - 1);
                Ray r = cam.getRay(u, v);
                col += color(r, scene, maxDepth);
            }
            //print colors and write to ppm file
            writeColor(std::cout, MyFile, col, samplesPerPixel);
        }
    }
     MyFile.close();
     std::cerr << "\nFinished!\n";
}