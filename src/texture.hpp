#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_
#include "./rtCommon.hpp"
#include "./perlinNoise.hpp"

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

class CheckerTexture : public Texture {

    public:
    CheckerTexture(){}

    CheckerTexture(shared_ptr<Texture> t0, shared_ptr<Texture> t1): even(t0), odd(t1) {}

    CheckerTexture(Vector3 color1, Vector3 color2) 
        :even(make_shared<SolidColor>(color1)), odd(make_shared<SolidColor>(color2)){}

    virtual Vector3 value(float u, float v, const Vector3& p) const override {
        auto sines = sin(10.0*p.getX())*sin(10.0*p.getY())*sin(10.0*p.getZ());
        if (sines < 0){
            return odd->value(u, v, p);
        }
        else {
            return even->value(u, v, p);
        }
    }

    public:
    shared_ptr<Texture> odd;
    shared_ptr<Texture> even;
    };


    class noiseTexture : public Texture {
        public: 
        noiseTexture(){}

        virtual Vector3 value(float u, float v, const Vector3& p) const override {
            return Vector3(1, 1, 1) * noise.noise(p);
        }

        public:
         PerlinNoise noise;
    };

#endif /* TEXTURE_HPP_*/