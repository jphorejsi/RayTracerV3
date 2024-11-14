#pragma once

#include "vec.h"
#include "color.h"
#include "materials.h"

class Scene;

// Abstract base class for lights and illumination sources with color attribute
class AbstractLight {
protected:
    Color color;

public:
    // Constructor
    AbstractLight(const Color& color) : color(color) {}

    // Getters
    Color getColor() const { return this->color; }

    // Setters
    void setColor(const Color& color) { this->color = color; }

    // Virtual functions for illumination
    virtual bool illuminates(const Vec3& position, const Scene& scene) const = 0;
    virtual Color calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const = 0;
    virtual Color calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const = 0;
};

// Concrete class for DirectionalLight with direction
class DirectionalLight : public AbstractLight {
protected:
    Vec3 direction;

public:
    // Constructor
    DirectionalLight(const Vec3& direction, const Color& color) : AbstractLight(color), direction(direction) {}

    // Getters
    Vec3 getDirection() const { return this->direction; }

    // Setters
    void setDirection(const Vec3& direction) { this->direction = direction; }

    // Overrides
    bool illuminates(const Vec3& position, const Scene& scene) const override;
    Color calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const override;
    Color calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const override;
};

// Concrete class for attenuating directional light
class AttributeDirectionalLight : public DirectionalLight {
protected:
    float c1, c2, c3;

public:
    // Constructor
    AttributeDirectionalLight(const Vec3& direction, const Color& color, float c1, float c2, float c3) : DirectionalLight(direction, color), c1(c1), c2(c2), c3(c3) {}

    // Getters
    float getC1() const { return this->c1; }
    float getC2() const { return this->c2; }
    float getC3() const { return this->c3; }

    // Setters
    void setC1(float value) { this->c1 = value; }
    void setC2(float value) { this->c2 = value; }
    void setC3(float value) { this->c3 = value; }

    // Overrides
    Color calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const override;
    Color calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const override;
};

// Concrete class for PointLight with position
class PointLight : public AbstractLight {
protected:
    Vec3 position;

public:
    // Constructor
    PointLight(const Vec3& position, const Color& color) : AbstractLight(color), position(position) {}

    // Getters
    Vec3 getPosition() const { return this->position; }

    // Setters
    void setPosition(const Vec3& position) { this->position = position; }

    // Overrides
    bool illuminates(const Vec3& targetPosition, const Scene& scene) const override;
    Color calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const override;
    Color calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const override;
};

// Concrete class for attenuating point light
class AttributePointLight : public PointLight {
protected:
    float c1, c2, c3;

public:
    // Constructor
    AttributePointLight(const Vec3& position, const Color& color, float c1, float c2, float c3) : PointLight(position, color), c1(c1), c2(c2), c3(c3) {}

    // Getters
    float getC1() const { return this->c1; }
    float getC2() const { return this->c2; }
    float getC3() const { return this->c3; }

    // Setters
    void setC1(float value) { this->c1 = value; }
    void setC2(float value) { this->c2 = value; }
    void setC3(float value) { this->c3 = value; }

    // Overrides
    Color calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const override;
    Color calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const override;
};
