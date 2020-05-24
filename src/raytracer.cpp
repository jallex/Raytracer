#include <iostream>
#include "vec3.hpp"

int main(int argc, char** args) {

//compile code

    Vect3F vect1(0.25);
    Vect3F vect2(0.25, 0.5, 0.25);

    float scalar = 2;

    vect1.normalize();
    
    //new
    int nx = 200;
    int ny = 100;

    std::cout <<"P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; j--){
        for(int i = 0; i < nx; i++){
            Vect3F col(float(i) / float(nx), float(j) / float(ny), 0.2);
            int ir = int(255.99*col.get_x());
            int ig = int(255.99*col.get_y());
            int ib = int(255.99*col.get_z());

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}