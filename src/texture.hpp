#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_
#include "./rtCommon.hpp"

class Texture {
    public:
        //Vector3 in this case represents color
        virtual Vector3 value(float u, float v, const Vector3&p) const = 0;
};

class SolidColor : public Texture {
    public:
    SolidColor(){}
    SolidColor(Vector3 c) : colorValue(c){}

    SolidColor(float r, float g, float b) : SolidColor(Vector3(r, g, b)){}

    virtual Vector3 value(float u, float v, const Vector3& p) const override {
        return colorValue;
    }

    private:
    Vector3 colorValue;
};
#endif /* TEXTURE_HPP_*/