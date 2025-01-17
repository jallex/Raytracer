#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_


#include "./geometry.hpp"
#include "./texture.hpp"


class Material {
    public:
    virtual bool scatter(const Ray& rayIn, const hitRecord& rec, 
    Vector3& attenuation, Ray& scattered) const=0;
    //emissive material
    virtual Vector3 emitted(float u, float v, const Vector3& p) const {
            //return white
            return Vector3(0,0,0);
        }
};

//Diffuse
class Lambertian : public Material {
    public:
    Lambertian(const Vector3& a) : albedo(make_shared<SolidColor>(a)) {}
    Lambertian(shared_ptr<Texture> a) : albedo(a) {}

    virtual bool scatter(const Ray& rayIn, const hitRecord& rec, 
    Vector3& attenuation, Ray& scattered) const override {
        //Light scatter
        //calculate object color
        //diffuse method 1
        // Vector3 scatterDirection = rec.p + rec.normal + randomInUnitSphere();
        //diffuse method 2
        Vector3 scatterDirection = rec.normal + randomUnitVec(); //Lambertian distribution
        //diffuse method 3
        //Vector3 scatterDirection = rec.p + randomInHemisphere(rec.normal);
        scattered = Ray(rec.p, scatterDirection, rayIn.getTime());
        //reduction or loss in the strength of the light with increasing distance from the light
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
    
    public:
    //the measure of the diffuse reflection of light 
    shared_ptr<Texture> albedo;
};


//Metal
class Metal : public Material {
    public:
    Metal(const Vector3& a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    virtual bool scatter(const Ray& rayIn, const hitRecord& rec, 
    Vector3& attenuation, Ray& scattered) const {
        //Light reflected
        Vector3 reflected = reflect(unitVector(rayIn.direction()), rec.normal);
        scattered = Ray(rec.p, randomInUnitSphere()*fuzz + reflected, rayIn.getTime());
        attenuation = albedo;
        return scattered.direction().dotProduct(rec.normal) > 0;
    }
    Vector3 albedo;
    float fuzz;
};

//Schlick's approximation for fresnel equations
//F = F0 + (1-F0)*(1-(dot-product(n, v))^5
//Where F0 is the reflectance at normal incidence, given by:
//F0= (n-1)^2 / (n+1)^2
float schlick(float cosine, float indexofRefraction){
    auto f0 = (pow((1-indexofRefraction),2)) / (pow((1+indexofRefraction),2));
    return f0 + (1-f0)*pow((1-cosine), 5);
}

//Dielectric (water, glass, diamonds...)
class Dielectric : public Material {
    public:
    Dielectric(float ir) : indexOfRefraction(ir){}

    virtual bool scatter(const Ray& rayIn, const hitRecord& rec, 
    Vector3& attenuation, Ray& scattered) const {
        //attenuation is always 1 because the glass absorbs nothing
        attenuation = Vector3(1.0, 1.0, 1.0);
        float etaIOverEtaT;
        //if this is the outside face 
        if(rec.frontFace) {
            etaIOverEtaT = 1.0 / indexOfRefraction;
        }
        else {
            etaIOverEtaT = indexOfRefraction;
        }

        Vector3 unitDirection = unitVector(rayIn.direction());
        float cosTheta = fmin((unitDirection*(-1)).dotProduct(rec.normal), 1.0);
        float sinTheta = sqrt(1.0-cosTheta*cosTheta);
        if(etaIOverEtaT*sinTheta > 1.0) {
            Vector3 reflected = reflect(unitDirection, rec.normal);
            scattered = Ray(rec.p, reflected, rayIn.getTime());
            return true;
        }
        //calculate reflectivity varying with angle 
        float reflectProb = schlick(cosTheta, etaIOverEtaT);
        if(randomNum() < reflectProb) {
            Vector3 reflected = reflect(unitDirection, rec.normal);
            scattered = Ray(rec.p, reflected, rayIn.getTime());
            return true;
        }
        Vector3 refracted = refract(unitDirection, rec.normal, etaIOverEtaT);
        scattered = Ray(rec.p, refracted, rayIn.getTime());
        return true;
    }

    //dimensionless number that describes how fast light travels through the material
    float indexOfRefraction;
};

class DiffuseLight : public Material {
    public:
        DiffuseLight(shared_ptr<Texture> a) : emit(a) {}
        DiffuseLight(Vector3 c) : emit(make_shared<SolidColor>(c)) {}

        virtual bool scatter(
            const Ray& r_in, const hitRecord& rec, Vector3& attenuation, Ray& scattered
        ) const override {
            return false;
        }

        virtual Vector3 emitted(float u, float v, const Vector3& p) const override {
            return emit->value(u, v, p);
        }

    public:
        shared_ptr<Texture> emit;
};

#endif /* MATERIAL_HPP_*/