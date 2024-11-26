#pragma once

#include "materials.h"
#include "color.h"
#include <memory> // For std::shared_ptr

class MaterialFactory {
public:
    static std::shared_ptr<IMaterial> createMaterial(const Color& od, const Color& os, double ka = 0, double kd = 0, double ks = 0, double n = 0);
    static std::shared_ptr<IMaterial> createReflectiveMaterial(const Color& od, const Color& os, double ka, double kd, double ks, double n, double alpha, double eta);
};
