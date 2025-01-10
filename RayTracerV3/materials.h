#pragma once
#include <memory>
#include "Color.h"
#include "ray.h"
#include "texture.h"

// Forward declaration
class Scene;
class Shape;

class Material {
protected:
    std::shared_ptr<Texture> texture;
    std::shared_ptr<NormalMap> normalMap;
public:
    virtual ~Material() = default;

    // Getters
    const Texture* getTexture() const { return texture.get(); }
    const NormalMap* getNormalMap() const { return normalMap.get(); }
    const double getAlpha() const { return 1.0;  }

    // Setters
    void setTexture(const std::shared_ptr<Texture>& texture) { this->texture = texture; }
    void setNormalMap(const std::shared_ptr<NormalMap>& normalMap) { this->normalMap = normalMap; }

    // Other methods
    virtual Color shade(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const Shape& shape) const = 0;
};

class BlinnPhong : public Material {
private:
    double ka; // Ambient coefficient between 0 and 1
    double kd; // Diffuse coefficient between 0 and 1
    double ks; // Specular coefficient between 0 and 1
    double n;  // Shininess exponent greater than 1
public:
    Color od;  // Diffuse color
    Color os;  // Specular color

    // Constructor
    BlinnPhong(Color od, Color os, double ka, double kd, double ks, double n);

    // Getters
    Color getOd(Vec2 textureCoordinate) const {
        if (this->getTexture()) {
            return this->texture->getPixel(textureCoordinate);
        }
        else return od;
    }

    double getKa() const { return ka; }
    double getKd() const { return kd; }
    double getKs() const { return ks; }
    double getN() const { return n; }


    // Setters
    void setKa(double ka);
    void setKd(double kd);
    void setKs(double ks);
    void setN(double n);

    // Overrides
    Color shade(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const Shape& shape) const override;

    //Color goraudShade()
};


//class ReflectiveBlinnPhong : public BlinnPhong {
//private:
//    double alpha = 1.0;
//    double eta = 1.0;
//
//public:
//    // Getters
//    double getAlpha() const { return alpha; }
//    double getEta() const { return eta; }
//
//    // Overrides
//    bool scatter(const Ray& r_in, const Vec3 intersectionPoint, Color& attenuation, Ray& scattered);
//};
