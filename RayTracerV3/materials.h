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
    virtual Color shadeRay(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const AbstractShape* shape) const = 0;

    // Virtual destructor
    virtual ~IMaterial() = default;
};

// RGBMaterial class with default white color
class RGBMaterial : public IMaterial {
private:
    Color color;

public:
    // Constructor with default color white
    RGBMaterial(Color color);

    // Getter
    Color getColor() const { return color; }

    // Setter
    void setColor(Color color);

    // Overrides
    Color shadeRay(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const AbstractShape* shape) const override;
};

// PhongMaterial class for Phong shading
class PhongMaterial : public IMaterial {
private:
    Color od;  // Diffuse color
    Color os;  // Specular color
    double ka;  // Ambient coefficient
    double kd;  // Diffuse coefficient
    double ks;  // Specular coefficient
    double n;   // Shininess exponent

public:
    // Constructor
    PhongMaterial(Color od, Color os, double ka, double kd, double ks, double n);

    // Getters
    Color getOd() const { return od; }
    Color getOs() const { return os; }
    double getKa() const { return ka; }
    double getKd() const { return kd; }
    double getKs() const { return ks; }
    double getN() const { return n; }

    // Setters
    void setOd(Color od);
    void setOs(Color os);
    void setKa(double ka);
    void setKd(double kd);
    void setKs(double ks);
    void setN(double n);

    // Overrides
    Color shadeRay(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const AbstractShape* shape) const override;
};

// ReflectivePhongMaterial class for reflective materials with Phong shading
class ReflectivePhongMaterial : public PhongMaterial {
private:
    double alpha; // Reflectivity
    double eta;   // Refractive index

public:
    // Constructor
    ReflectivePhongMaterial(Color od, Color os, double ka, double kd, double ks, double n, double alpha, double eta);

    // Getters
    double getAlpha() const { return alpha; }
    double getEta() const { return eta; }

    // Setters
    void setAlpha(double alpha);
    void setEta(double eta);

    // Overrides
    Color shadeRay(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const AbstractShape* shape) const override;
};
