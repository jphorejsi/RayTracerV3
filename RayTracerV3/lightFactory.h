#pragma once

#include "lights.h"
#include "vec.h"
#include "color.h"

// LightFactory for creating different types of lights
class LightFactory {
public:
    static ILight* createPointLight(const Vec3& position, const Color& color);
    static ILight* createDirectionalLight(const Vec3& direction, const Color& color);
    static ILight* createAttributePointLight(const Vec3& position, const Color& color, float c1, float c2, float c3);
    static ILight* createAttributeDirectionalLight(const Vec3& direction, const Color& color, float c1, float c2, float c3);
};
