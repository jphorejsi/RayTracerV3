#pragma once
#include "Color.h"

class Material {
private:
    Color od;  // Diffuse color
    Color os;  // Specular color
    double ka; // Ambient coefficient
    double kd; // Diffuse coefficient
    double ks; // Specular coefficient
    double n;  // Shininess exponent

    // Reflective/Refractive properties
    double alpha; // Reflectivity (default: 0)
    double eta;   // Refractive index (default: 1)

public:
    // Constructor with default arguments defined here
    Material(Color od, Color os, double ka, double kd, double ks, double n, double alpha = 0.0, double eta = 1.0);

    // Getters
    Color getOd() const { return od; }
    Color getOs() const { return os; }
    double getKa() const { return ka; }
    double getKd() const { return kd; }
    double getKs() const { return ks; }
    double getN() const { return n; }
    double getAlpha() const { return alpha; }
    double getEta() const { return eta; }

    // Setters
    void setOd(Color od);
    void setOs(Color os);
    void setKa(double ka);
    void setKd(double kd);
    void setKs(double ks);
    void setN(double n);
    void setAlpha(double alpha);
    void setEta(double eta);
};
