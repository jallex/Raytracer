#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_


#include "./geometry.hpp"


class Material {
    public:
    virtual bool scatter(const Ray& ray_in, const hit_record& rec, 
    Vector3& attebnuation, Ray& scattered) const=0;
};

//Diffuse
class Lambertian : public Material {
    public:
    Lambertian(const Vector3& a) : albedo(a) {}

    virtual bool scatter(const Ray& ray_in, const hit_record& rec, 
    Vector3& attenuation, Ray& scattered) const {
        //Light scatter
        //calculate object color
        //diffuse method 1
        // Vector3 scatter_direction = rec.p + rec.normal + random_in_unit_sphere();
        //diffuse method 2
        Vector3 scatter_direction = rec.p + rec.normal + random_unit_vec(); //Lambertian distribution
        //diffuse method 3
        //Vector3 scatter_direction = rec.p + random_in_hemisphere(rec.normal);
        scattered = Ray(rec.p, scatter_direction);
        //reduction or loss in the strength of the light with increasing distance from the light
        attenuation = albedo;
        return true;
    }
    //the measure of the diffuse reflection of light 
    Vector3 albedo;
};


//Metal
class Metal : public Material {
    public:
    Metal(const Vector3& a, float f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    virtual bool scatter(const Ray& ray_in, const hit_record& rec, 
    Vector3& attenuation, Ray& scattered) const {
        //Light reflected
        Vector3 reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
        scattered = Ray(rec.p, random_in_unit_sphere()*fuzz + reflected);
        attenuation = albedo;
        return scattered.direction().dot_product(rec.normal) > 0;
    }
    Vector3 albedo;
    float fuzz;
};

//Dielectric (water, glass, diamonds...)
class Dialectric : public Material {
    public:
    Dialectric(float ir) : index_of_refraction(ir){}

    virtual bool scatter(const Ray& ray_in, const hit_record& rec, 
    Vector3& attenuation, Ray& scattered) const {
        attenuation = Vector3(1.0, 1.0, 1.0);
        float etaI_over_etaT;
        //if this is the outside face 
        if(rec.front_face) {
            etaI_over_etaT = 1.0 / index_of_refraction;
        }
        else {
            etaI_over_etaT = index_of_refraction;
        }

        Vector3 unit_direction = unit_vector(ray_in.direction());
        Vector3 refracted = refract(unit_direction, rec.normal, etaI_over_etaT);
        scattered = Ray(rec.p, refracted);
        return true;
    }
    //dimensionless number that describes how fast light travels through the material
    float index_of_refraction;
};

#endif /* MATERIAL_HPP_*/