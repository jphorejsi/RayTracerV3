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
    double c1, c2, c3;

public:
    // Constructor
    AttributeDirectionalLight(const Vec3& direction, const Color& color, double c1, double c2, double c3) : DirectionalLight(direction, color), c1(c1), c2(c2), c3(c3) {}

    // Getters
    double getC1() const { return this->c1; }
    double getC2() const { return this->c2; }
    double getC3() const { return this->c3; }

    // Setters
    void setC1(double value) { this->c1 = value; }
    void setC2(double value) { this->c2 = value; }
    void setC3(double value) { this->c3 = value; }

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
    double c1, c2, c3;

public:
    // Constructor
    AttributePointLight(const Vec3& position, const Color& color, double c1, double c2, double c3) : PointLight(position, color), c1(c1), c2(c2), c3(c3) {}

    // Getters
    double getC1() const { return this->c1; }
    double getC2() const { return this->c2; }
    double getC3() const { return this->c3; }

    // Setters
    void setC1(double value) { this->c1 = value; }
    void setC2(double value) { this->c2 = value; }
    void setC3(double value) { this->c3 = value; }

    // Overrides
    Color calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const override;
    Color calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const override;
};
