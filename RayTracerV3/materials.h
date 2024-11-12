#pragma once

#include "vec.h"
#include "color.h"
#include "ray.h"

// Forward declarations
class Scene;
class AbstractShape;

// Abstract base class for materials
class IMaterial {
public:
    // Constructor
    IMaterial() = default;

    // Virtual methods
    virtual Color shade(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const AbstractShape* shape) const = 0;
};

// RGBMaterial class with default white color
class RGBMaterial : public IMaterial {
private:
    Color color;

public:
    // Constructor with default color white
    RGBMaterial(Color color) : color(color) {}

    // Overrides
    Color shade(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const AbstractShape* shape) const override { return color; }
};

// PhongMaterial class for Phong shading
class PhongMaterial : public IMaterial {
private:
    Color od;  // Diffuse color
    Color os;  // Specular color
    float ka;  // Ambient coefficient
    float kd;  // Diffuse coefficient
    float ks;  // Specular coefficient
    float n;   // Shininess exponent

public:
    // Constructor
    PhongMaterial(Color od, Color os, float ka, float kd, float ks, float n) : od(od), os(os), ka(ka), kd(kd), ks(ks), n(n) {}

    // Getters
    Color getOd() const { return od; }
    Color getOs() const { return os; }
    float getKa() const { return ka; }
    float getKd() const { return kd; }
    float getKs() const { return ks; }
    float getN() const { return n; }

    // Setters
    void setOd(Color od) { this->od = od; }
    void setOs(Color os) { this->os = os; }
    void setKa(float ka) { this->ka = ka; }
    void setKd(float kd) { this->kd = kd; }
    void setKs(float ks) { this->ks = ks; }
    void setN(float n) { this->n = n; }

    // overrides
    Color shade(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const AbstractShape* shape) const override;
};