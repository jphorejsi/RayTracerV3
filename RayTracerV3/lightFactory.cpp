#include "lightFactory.h"

#include <stdexcept>

// Factory method to create a PointLight with position
AbstractLight* LightFactory::createPointLight(const Vec3& position, const Color& color) {
    // Check if color values are within the range [0, 1]
    if (color.getR() < 0 || color.getR() > 1 || color.getG() < 0 || color.getG() > 1 || color.getB() < 0 || color.getB() > 1) {
        throw std::runtime_error("Error: Light color values must be between 0 and 1.");
    }
    return new PointLight(position, color);
}

// Factory method to create a DirectionalLight with direction
AbstractLight* LightFactory::createDirectionalLight(const Vec3& direction, const Color& color) {
    // Check if color values are within the range [0, 1]
    if (color.getR() < 0 || color.getR() > 1 || color.getG() < 0 || color.getG() > 1 || color.getB() < 0 || color.getB() > 1) {
        throw std::runtime_error("Error: Light color values must be between 0 and 1.");
    }
    return new DirectionalLight(direction, color);
}

// Factory method to create an AttributePointLight with attributes
AbstractLight* LightFactory::createAttributePointLight(const Vec3& position, const Color& color, double c1, double c2, double c3) {
    // Check if color values are within the range [0, 1]
    if (color.getR() < 0 || color.getR() > 1 || color.getG() < 0 || color.getG() > 1 || color.getB() < 0 || color.getB() > 1) {
        throw std::runtime_error("Error: Attribute light color values must be between 0 and 1.");
    }
    // Check if attenuation coefficients are non-negative
    if (c1 < 0 || c2 < 0 || c3 < 0) {
        throw std::runtime_error("Error: Attenuation coefficients must be non-negative.");
    }
    return new AttributePointLight(position, color, c1, c2, c3);
}

// Factory method to create an AttributeDirectionalLight with attributes
AbstractLight* LightFactory::createAttributeDirectionalLight(const Vec3& direction, const Color& color, double c1, double c2, double c3) {
    // Check if color values are within the range [0, 1]
    if (color.getR() < 0 || color.getR() > 1 || color.getG() < 0 || color.getG() > 1 || color.getB() < 0 || color.getB() > 1) {
        throw std::runtime_error("Error: Attribute light color values must be between 0 and 1.");
    }
    // Check if attenuation coefficients are non-negative
    if (c1 < 0 || c2 < 0 || c3 < 0) {
        throw std::runtime_error("Error: Attenuation coefficients must be non-negative.");
    }
    return new AttributeDirectionalLight(direction, color, c1, c2, c3);
}
