#ifndef IMAGETEXTURE_HPP_
#define IMAGETEXTURE_HPP_

#include "./rtCommon.hpp"
#include "./rtSTBImage.h"
#include "./perlinNoise.hpp"
#include "./texture.hpp"
#include <string>

#include <iostream>

class ImageTexture : public Texture {
    public:
        const static int bytesPerPixel = 3;

        ImageTexture(const char* fileName) {
            auto componentsPerPixel = bytesPerPixel;

            std::cout << fileName;
        
        data = stbi_load(fileName, &width, &height, &componentsPerPixel, componentsPerPixel);

        if(!data) {
            std::cout <<"image not loaded.";
            std::cerr<<"Error: Problem loading texture image file: " << fileName << "\n";
            width = 0;
            height = 0;
        }

        else{
            std::cout <<"image loaded.";
            std::cerr<< data;
        }

        bytesPerScanline = bytesPerPixel * width;
    }

    virtual Vector3 value(float u, float v, const Vector3& p) const override {
        //if no texture data is recorded, return cyan to help debug 
        if(data == nullptr){
            return Vector3(0, 1, 1);
        }

        //Clamp input texture coordinates to [0,1] x [1,0]
        u=restrictColor(u, 0.0, 1.0);
        //Convert V to image coordinates
        v = 1.0 - restrictColor(v, 0.0, 1.0);

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);   

        //Coordinates should be less than 1.0, so clamp integer mapping
        if(i >= width) {
            i = width - 1;        
        }
        if(j >= height) {
            j = height - 1;
        }

        const auto colorScale = 1.0 / 255.0;

        auto pixel = data + j*bytesPerScanline + i*bytesPerPixel; 

        return Vector3(colorScale*pixel[0], colorScale*pixel[1], colorScale*pixel[2]);
    }

    ~ImageTexture() {
        delete data;
    }

    private:
    unsigned char *data;
    int width;
    int height;
    int bytesPerScanline;
};


#endif /* IMAGETEXTURE_HPP_*/