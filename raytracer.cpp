#include "vec3.hpp"

int main(int argc, char** args) {

//compile code

    Vect3F vect1(0.25);
    Vect3F vect2(0.25, 0.5, 0.25);

    float scalar = 2;

    vect1.normalize();

    std::cout << vect1 * scalar << std::endl;

}