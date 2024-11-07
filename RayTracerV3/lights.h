#pragma once

#include "vec.h"
#include "color.h"

// Abstract base class for lights and illumination sources
class ILight {
public:
    virtual ~ILight() = default;
    virtual bool illuminates(const Vec3& position) const = 0;
};

// Concrete class for DirectionalLight with color and direction
class DirectionalLight : public ILight {
protected:
    Vec3 direction;
    Color color;

public:
    // Constructor
    DirectionalLight(const Vec3& direction, const Color& color) : direction(direction), color(color) {}

    // Getters
    Vec3 getDirection() const { return direction; }
    Color getColor() const { return this->color; }

    // Setters
    void setDirection(const Vec3& direction) { this->direction = direction; }
    void setColor(const Color& color) { this->color = color; }

    // Overrides
    bool illuminates(const Vec3& position) const override { return true; }
};

// Concrete class for PointLight with color and position
class PointLight : public ILight {
protected:
    Vec3 position;
    Color color;

public:
    // Constructor
    PointLight(const Vec3& position, const Color& color) : position(position), color(color) {}

    // Getters
    Vec3 getPosition() const { return position; }
    Color getColor() const { return this->color; }

    // Setters
    void setPosition(const Vec3& position) { this->position = position; }
    void setColor(const Color& color) { this->color = color; }

    // Overrides
    bool illuminates(const Vec3& position) const override { return true; }
};

// Concrete class for AttributeDirectionalLight with color, direction, and attenuation factors
class AttributeDirectionalLight : public ILight {
protected:
    Vec3 direction;
    Color color;
    float c1, c2, c3;

public:
    // Constructor
    AttributeDirectionalLight(const Vec3& direction, const Color& color, float c1, float c2, float c3) : direction(direction), color(color), c1(c1), c2(c2), c3(c3) {}

    // Getters
    Vec3 getDirection() const { return this->direction; }
    Color getColor() const { return this->color; }
    float getC1() const { return this->c1; }
    float getC2() const { return this->c2; }
    float getC3() const { return this->c3; }

    // Setters
    void setDirection(const Vec3& direction) { this->direction = direction; }
    void setColor(const Color& color) { this->color = color; }
    void setC1(float value) { this->c1 = value; }
    void setC2(float value) { this->c2 = value; }
    void setC3(float value) { this->c3 = value; }

    // Overrides
    bool illuminates(const Vec3& targetPosition) const override { return true; }
};

// Concrete class for AttributePointLight with color, position, and attenuation factors
class AttributePointLight : public ILight {
protected:
    Vec3 position;
    Color color;
    float c1, c2, c3;

public:
    // Constructor
    AttributePointLight(const Vec3& position, const Color& color, float c1, float c2, float c3) : position(position), color(color), c1(c1), c2(c2), c3(c3) {}

    // Getters
    Vec3 getPosition() const { return this->position; }
    Color getColor() const { return this->color; }
    float getC1() const { return this->c1; }
    float getC2() const { return this->c2; }
    float getC3() const { return this->c3; }

    // Setters
    void setPosition(const Vec3& position) { this->position = position; }
    void setColor(const Color& color) { this->color = color; }
    void setC1(float value) { this->c1 = value; }
    void setC2(float value) { this->c2 = value; }
    void setC3(float value) { this->c3 = value; }

    // Overrides
    bool illuminates(const Vec3& targetPosition) const override { return true; }
};