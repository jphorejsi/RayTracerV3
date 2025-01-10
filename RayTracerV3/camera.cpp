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

void Camera::setUpDirection(const Vec3 upDir) {
    if (upDir.length() == 0.0) {
        throw std::invalid_argument("Up direction vector cannot be zero.");
    }
    this->upDirection = upDir.normal(); // Normalize before setting
}

void Camera::setViewDirection(const Vec3 viewDir) {
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

void Camera::createFrustrum(int imageWidth, int imageHeight) {
    // Calculate the aspect ratio
    double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);

    // Compute the window width based on the camera's horizontal field of view (FOV).
    // Convert FOV from degrees to radians: (FOV * pi / 180)
    double fovRadians = horizontalFOV * 3.14159265358979323846 / 180.0;
    this->VFWidth = 2.0 * 5.0 * tan(0.5 * fovRadians); // Assuming a view plane distance of 5 units

    // Calculate the window height using the aspect ratio.
    this->VFHeight = this->VFWidth / aspectRatio;

    // Compute the camera's local axes
    Vec3 u = cross(viewDirection, upDirection).normal() * (VFWidth / 2.0);
    Vec3 v = cross(u, viewDirection).normal() * (VFHeight / 2.0);

    // Compute the center of the view plane
    Vec3 viewPlaneCenter = eyePosition + viewDirection * 5.0; // Assuming view plane distance of 5 units

    // Compute the four corners of the viewing window
    this->VFUpperLeft = viewPlaneCenter - u + v;
    this->VFUpperRight = viewPlaneCenter + u + v;
    this->VFLowerLeft = viewPlaneCenter - u - v;
    this->VFLowerRight = viewPlaneCenter + u - v;
}