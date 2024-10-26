#pragma once
#include "vec.h"
#include "color.h"

class LightType {
protected:
    ColorType color;

public:
    LightType(ColorType color) : color(color) {}
};


class DirectionalLightType : public LightType {
    Vec3 direction;

public:
    DirectionalLightType(Vec3 direction, ColorType color) : direction(direction), LightType(color) {}
};


class PointLightType : public LightType {
    Vec3 position;

public:
    PointLightType(Vec3 position, ColorType color) : position(position), LightType(color) {}
};


class AttributeDirectionalLightType : public DirectionalLightType {
    float c1, c2, c3; 

public:
    AttributeDirectionalLightType(Vec3 direction, ColorType color, float c1, float c2, float c3) : DirectionalLightType(direction, color), c1(c1), c2(c2), c3(c3) {}
};


class AttributePointLightType : public PointLightType {
    float c1, c2, c3;

public:
    AttributePointLightType(Vec3 point, ColorType color, float c1, float c2, float c3) : PointLightType(point, color), c1(c1), c2(c2), c3(c3) {}
};
