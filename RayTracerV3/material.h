#pragma once
#include "vec.h"
#include "color.h"
#include "ray.h"

// Abstract Base Class for Materials
class IMaterial {
public:
    virtual ~IMaterial() = default;
    virtual Color interactRay(const Ray& rayIn, const Vec3& intersectionPoint, const Vec3& normal, Ray& resultantRay) const = 0;
};

// RGBMaterial class with default white color
class RGBMaterial : public IMaterial {
private:
    Color color;

public:
    // Constructor with default color white
    RGBMaterial(Color color = Color(1, 1, 1)) : color(color) {}

    // Override interactRay to simply return the color
    Color interactRay(const Ray& rayIn, const Vec3& intersectionPoint, const Vec3& normal, Ray& resultantRay) const override {
        return color;
    }

    // Getter for color
    Color getColor() const { return color; }

    // Setter for color
    void setColor(const Color& newColor) { color = newColor; }
};

// PhongMaterial class for Phong shading
class PhongMaterial : public IMaterial {
private:
    Color od;  // Diffuse color
    Color os;  // Specular color
    float ka;  // Ambient coefficient
    float kd;  // Diffuse coefficient
    float ks;  // Specular coefficient
    float n;   // Phong exponent

public:
    // Constructor
    PhongMaterial(Color od, Color os, float ka = 0, float kd = 0, float ks = 0, float n = 0) : od(od), os(os), ka(ka), kd(kd), ks(ks), n(n) {}

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

    // Override interactRay
    Color interactRay(const Ray& rayIn, const Vec3& intersectionPoint, const Vec3& normal, Ray& resultantRay) const override {
        // Placeholder for Phong shading implementation
        return Color(1, 1, 1); // Change as needed for actual shading calculations
    }
};


class MaterialFactory {
public:
    static IMaterial* createRGBMaterial(const Color& color);
    static IMaterial* createPhongMaterial(const Color& od, const Color& os, float ka = 0, float kd = 0, float ks = 0, float n = 0);
};