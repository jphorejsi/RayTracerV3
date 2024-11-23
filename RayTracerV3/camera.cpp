#include "Camera.h"
#include <stdexcept> // For std::invalid_argument

// Constructor with validation
Camera::Camera(const Vec3& eyePosition, const Vec3& upDirection, const Vec3& viewDirection, double horizontalFOV) {
    if (horizontalFOV <= 0.0 || horizontalFOV >= 180.0) {
        throw std::invalid_argument("Horizontal FOV must be between 0 and 180 degrees (exclusive).");
    }
    if (viewDirection.length() == 0.0) {
        throw std::invalid_argument("View direction vector cannot be zero.");
    }
    if (upDirection.length() == 0.0) {
        throw std::invalid_argument("Up direction vector cannot be zero.");
    }

    this->eyePosition = eyePosition;
    this->upDirection = upDirection.normal(); // Normalize the up direction
    this->viewDirection = viewDirection.normal(); // Normalize the view direction
    this->horizontalFOV = horizontalFOV;
}

// Setters with validation
void Camera::setEyePosition(const Vec3& eyePos) {
    this->eyePosition = eyePos; // No validation needed for eye position
}

void Camera::setUpDirection(const Vec3& upDir) {
    if (upDir.length() == 0.0) {
        throw std::invalid_argument("Up direction vector cannot be zero.");
    }
    this->upDirection = upDir.normal(); // Normalize before setting
}

void Camera::setViewDirection(const Vec3& viewDir) {
    if (viewDir.length() == 0.0) {
        throw std::invalid_argument("View direction vector cannot be zero.");
    }
    this->viewDirection = viewDir.normal(); // Normalize before setting
}

void Camera::setHorizontalFOV(double fov) {
    if (fov <= 0.0 || fov >= 180.0) {
        throw std::invalid_argument("Horizontal FOV must be between 0 and 180 degrees (exclusive).");
    }
    this->horizontalFOV = fov;
}
