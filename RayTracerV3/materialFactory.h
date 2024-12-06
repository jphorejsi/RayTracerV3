#pragma once

#include "materials.h"
#include "color.h"

class MaterialFactory {
public:
    static Material* createMaterial(const Color& od, const Color& os, double ka = 0, double kd = 0, double ks = 0, double n = 0);
    static Material* createReflectiveMaterial(const Color& od, const Color& os, double ka, double kd, double ks, double n, double alpha, double eta);
};
