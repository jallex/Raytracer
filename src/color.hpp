
#ifndef COLOR_HPP_
#define COLOR_HPP_

#include "./rtCommon.hpp"
#include <iostream>
#include <fstream>



void writeColor(std::ostream &out, std::ofstream &MyFile, Vector3 pixelColor, int samplesPerPixel) {
    auto r = pixelColor.getX();
    auto g = pixelColor.getY();
    auto b = pixelColor.getZ();

// Divide the color total by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samplesPerPixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
     MyFile << static_cast<int>(256 * restrictColor(r, 0.0, 0.999)) 
        << ' '
        << static_cast<int>(256 * restrictColor(g, 0.0, 0.999)) 
        << ' '
        << static_cast<int>(256 * restrictColor(b, 0.0, 0.999)) << '\n';
}

#endif /* COLOR_HPP_*/