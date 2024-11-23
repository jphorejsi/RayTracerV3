#pragma once

#include "lights.h"
#include "vec.h"
#include "color.h"

// LightFactory for creating different types of lights
class LightFactory {
public:
    static AbstractLight* createPointLight(const Vec3& position, const Color& color);
    static AbstractLight* createDirectionalLight(const Vec3& direction, const Color& color);
    static AbstractLight* createAttributePointLight(const Vec3& position, const Color& color, double c1, double c2, double c3);
    static AbstractLight* createAttributeDirectionalLight(const Vec3& direction, const Color& color, double c1, double c2, double c3);
};
