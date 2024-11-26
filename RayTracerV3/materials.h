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


    // Virtual destructor
    virtual ~IMaterial() = default;
};

// Material class for Phong shading
class Material : public IMaterial {
protected:
    Color od;  // Diffuse color
    Color os;  // Specular color
    double ka;  // Ambient coefficient
    double kd;  // Diffuse coefficient
    double ks;  // Specular coefficient
    double n;   // Shininess exponent

public:
    // Constructor
    Material(Color od, Color os, double ka, double kd, double ks, double n);

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
};

// ReflectiveMaterial class for reflective materials with Phong shading
class ReflectiveMaterial : public Material {
protected:
    double alpha; // Reflectivity
    double eta;   // Refractive index

public:
    // Constructor
    ReflectiveMaterial(Color od, Color os, double ka, double kd, double ks, double n, double alpha, double eta);

    // Getters
    double getAlpha() const { return alpha; }
    double getEta() const { return eta; }

    // Setters
    void setAlpha(double alpha);
    void setEta(double eta);
};
