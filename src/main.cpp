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
#include "./movingSphere.hpp"
#include "./imageTexture.hpp"
#include "./XYRect.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <string>


using namespace std;

//calculate color at the set max depth
Vector3 color(const Ray& r, const Vector3 backgroundColor, const Geometry& scene, int depth) {
    hitRecord rec;
    if(depth <= 0) { 
        //no light
        return Vector3(0, 0, 0);
    }
    if(!scene.hit(r, 0.001, infinity, rec)) {
        //ray didn't hit any object, return background color
        return backgroundColor;
    }
        Ray scattered;
        Vector3 attenuation;
        Vector3 emitted = rec.matPtr->emitted(rec.u, rec.v, rec.p);

        //just light, not scattered on any object
        if (!rec.matPtr->scatter(r, rec, attenuation, scattered)){
            return emitted;
        }
    //object under light recursively find color
    return emitted + attenuation * color(scattered, backgroundColor, scene, depth-1);
}


//A random scene of spheres with different materials 
LoGeometry randomScene() {
    LoGeometry world;

    auto groundMaterial = make_shared<Lambertian>(Vector3(0.5, 0.5, 0.5));
    
    //checkered ground with constructor taking in the 2 colors 
    auto checkeredGround = make_shared<CheckerTexture>(Vector3(0.2, 0.3, 0.1), Vector3(0.9, 0.9, 0.9));
    world.add(make_shared<Sphere>(Vector3(0,-1000,0), 1000, make_shared<Lambertian>(checkeredGround)));

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
                    auto center2 = center + Vector3(0, randomNum(0, 0.5), 0);
                    world.add(make_shared<MovingSphere>(center, center2, 0.0, 1.0, 0.2, sphereMaterial));
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

//A scene with 2 checkered Spheres
LoGeometry checkeredSpheres(){
    LoGeometry objects;

    auto checkerTexture = make_shared<CheckerTexture>(Vector3(0.2, 0.3, 0.1), Vector3(0.9, 0.9, 0.9));
    objects.add(make_shared<Sphere>(Vector3(0, -10, 0), 10, make_shared<Lambertian>(checkerTexture)));
    objects.add(make_shared<Sphere>(Vector3(0, 10, 0), 10, make_shared<Lambertian>(checkerTexture)));
    return objects;
}

//A scene with two spheres
LoGeometry perlinSpheres(){
    LoGeometry objects;
    auto perlinTexture = make_shared<noiseTexture>(4);

    objects.add(make_shared<Sphere>(Vector3(0, -1000, 0), 1000, make_shared<Lambertian>(perlinTexture)));
    objects.add(make_shared<Sphere>(Vector3(0, 2, 0), 2, make_shared<Lambertian>(perlinTexture)));
    return objects;
}

//debugging file existence
inline bool exists_test3 (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

//A scene with projected image textures
LoGeometry planetsTextures(){
    auto jupiterTexture = make_shared<ImageTexture>("src/imageTextures/jupiter.jpg");
    auto jupiterSurface = make_shared<Lambertian>(jupiterTexture);
    auto sphere = make_shared<Sphere>(Vector3(0, 0, 0), 2, jupiterSurface);

    return LoGeometry(sphere);
}

//A scene with a rectangle acting as a light
LoGeometry rectLight(){
    LoGeometry objects;
    auto perlinTexture = make_shared<noiseTexture>(4);
    objects.add(make_shared<Sphere>(Vector3(0,-1000,0), 1000, make_shared<Lambertian>(perlinTexture)));
    objects.add(make_shared<Sphere>(Vector3(0,2,0), 2, make_shared<Lambertian>(perlinTexture)));

    auto difflight = make_shared<DiffuseLight>(Vector3(4,4,4));
    objects.add(make_shared<XYRect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

//main!
int main() {
    int sceneNumber = 5;
    //black background
    Vector3 backgroundColor(0,0,0);
    ofstream MyFile("myImage5.ppm");
    //new
    const auto aspectRatio = 16.0 / 9.0;
    //image width
    int width = 400;
    //image height
    int height = static_cast<int>(width / aspectRatio);
    int samplesPerPixel = 100;
    const int maxDepth = 50;

    std::cout <<"P3\n" << width << " " << height << "\n255\n";
     MyFile <<"P3\n" << width << " " << height << "\n255\n";

    //Create geometry
    LoGeometry scene;
    Vector3 lookfrom(13,2,3);
    Vector3 lookat(0,0,0);
    float vfov = 40.0;
    auto aperture = 0.0;

    //switch case to determine what scene to render
    switch(sceneNumber){
        case 1:
            scene = randomScene();
            backgroundColor = Vector3(0.70, 0.80, 1.00);
            lookfrom = Vector3(13, 2, 3);
            lookat = Vector3(0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        case 2: 
            scene = checkeredSpheres();
            backgroundColor = Vector3(0.70, 0.80, 1.00);
            lookfrom = Vector3(13, 2, 3);
            lookat = Vector3(0, 0, 0);
            vfov = 20.0;
            break;
        case 3:
            scene = perlinSpheres();
            backgroundColor = Vector3(0.70, 0.80, 1.00);
            lookfrom = Vector3(13, 2, 3);
            lookat = Vector3(0, 0, 0);
            vfov = 20.0;
            break;
        case 4: 
            scene = planetsTextures();
            lookfrom = Vector3(13, 2, 3);
            lookat = Vector3(0, 0, 0);
            vfov = 20.0;
            break;
        case 5:
            scene = rectLight();
            samplesPerPixel = 400;
            backgroundColor = Vector3(0,0,0);
            lookfrom = Vector3(26, 3, 6);
            lookat = Vector3(0,2,0);
            vfov = 20.0;
            break;
    }


    //Add camera to scene
    Vector3 vup(0,1,0);
    auto distToFocus = 10.0;

    Camera cam(lookfrom, lookat, vup, vfov, aspectRatio, aperture, distToFocus, 0.0, 1.0);

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
                col += color(r, backgroundColor, scene, maxDepth);
            }
            //print colors and write to ppm file
            writeColor(std::cout, MyFile, col, samplesPerPixel);
        }
    }
     MyFile.close();
     std::cerr << "\nFinished!\n";
}