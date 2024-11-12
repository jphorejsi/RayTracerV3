#pragma once

#include "lights.h"
#include "vec.h"
#include "color.h"

// LightFactory for creating different types of lights
class LightFactory {
public:
    static AbstractLight* createPointLight(const Vec3& position, const Color& color);
    static AbstractLight* createDirectionalLight(const Vec3& direction, const Color& color);
    static AbstractLight* createAttributePointLight(const Vec3& position, const Color& color, float c1, float c2, float c3);
    static AbstractLight* createAttributeDirectionalLight(const Vec3& direction, const Color& color, float c1, float c2, float c3);
};
