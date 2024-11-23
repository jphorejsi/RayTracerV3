#pragma once

#include "materials.h"
#include "color.h"

class MaterialFactory {
public:
    static IMaterial* createRGBMaterial(const Color& color);
    static IMaterial* createPhongMaterial(const Color& od, const Color& os, double ka = 0, double kd = 0, double ks = 0, double n = 0);
};