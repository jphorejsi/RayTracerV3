#pragma once

#include "materials.h"
#include "color.h"

class MaterialFactory {
public:
    static IMaterial* createRGBMaterial(const Color& color);
    static IMaterial* createPhongMaterial(const Color& od, const Color& os, float ka = 0, float kd = 0, float ks = 0, float n = 0);
};