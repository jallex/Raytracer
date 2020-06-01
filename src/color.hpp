
#ifndef COLOR_HPP_
#define COLOR_HPP_

#include "./rt_common.hpp"
#include <iostream>
#include <fstream>

void write_color(std::ostream &out, std::ofstream &MyFile, Vector3 pixel_color, int samples_per_pixel) {
    auto r = pixel_color.get_x();
    auto g = pixel_color.get_y();
    auto b = pixel_color.get_z();

    // Divide the color total by the number of samples.
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * restrict_color(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * restrict_color(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * restrict_color(b, 0.0, 0.999)) << '\n';


    MyFile << static_cast<int>(256 * restrict_color(r, 0.0, 0.999)) 
        << ' '
        << static_cast<int>(256 * restrict_color(g, 0.0, 0.999)) 
        << ' '
        << static_cast<int>(256 * restrict_color(b, 0.0, 0.999)) << '\n';
}

#endif /* COLOR_HPP_*/