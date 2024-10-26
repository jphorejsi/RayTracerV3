#pragma once
#include "vec.h"
#include "color.h"

class MaterialType {
private:
    ColorType od;  // Diffuse color
    ColorType os;  // Specular color
    float ka; // Ambient coefficient
    float kd; // Diffuse coefficient
    float ks; // Specular coefficient
    float n;  // Phong exponent

public:
    // Constructor
    MaterialType(ColorType od, ColorType os, float ka = 0, float kd = 0, float ks = 0, float n = 0) : od(od), os(os), ka(ka), kd(kd), ks(ks), n(n) {}

    // Getters
    ColorType getOd() const { return od; }
    ColorType getOs() const { return os; }
    float getKa() const { return ka; }
    float getKd() const { return kd; }
    float getKs() const { return ks; }
    float getN() const { return n; }

    // Setters
    void setOd(ColorType od) { this->od = od; }
    void setOs(ColorType os) { this->os = os; }
    void setKa(float ka) { this->ka = ka; }
    void setKd(float kd) { this->kd = kd; }
    void setKs(float ks) { this->ks = ks; }
    void setN(float n) { this->n = n; }

    // Other Methods
};
