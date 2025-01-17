#pragma once
#include <memory>
#include "Color.h"
#include "ray.h"
#include "texture.h"

// Forward declaration Scene includes shapes which include materials
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
    virtual Color shade(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const Shape& shape, int maxDepth) const = 0;
};

class BlinnPhong : public Material {
private:
    double ka;
    double kd;
    double ks;
    double n;

public:
    Color od;  // Diffuse color
    Color os;  // Specular color

    // Constructor
    BlinnPhong(Color od, Color os, double ka, double kd, double ks, double n);

    // Getters
    Color getOd(Vec2 textureCoordinate) const;
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
    virtual Color shade(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const Shape& shape, int maxDepth) const;
};


class ReflectiveBlinnPhong : public BlinnPhong {
private:
    double alpha = 1.0; // Transparency
    double eta = 1.0;   // Index of refraction

public:
    // Constructor
    ReflectiveBlinnPhong(Color od, Color os, double ka, double kd, double ks, double n, double alpha, double eta);

    // Getters
    double getAlpha() const { return alpha; }
    double getEta() const { return eta; }

    // Setters
    void setAlpha(double alpha);
    void setEta(double eta);

    // Overrides
    virtual Color shade(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const Shape& shape, int maxDepth) const;
};
