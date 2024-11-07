#pragma once
#include "vec.h"

class Camera {
private:
    Vec3 eyePosition;
    Vec3 upDirection;
    Vec3 viewDirection;
    float horizontalFOV;

public:
    // Constructors
    Camera() = default;
    Camera(const Vec3& eyePosition, const Vec3& upDirection, const Vec3& viewDirection, float horizontalFOV) : eyePosition(eyePosition), upDirection(upDirection), viewDirection(viewDirection), horizontalFOV(horizontalFOV) {}

    // Getters
    Vec3 getEyePosition() const { return eyePosition; }
    Vec3 getUpDirection() const { return upDirection; }
    Vec3 getViewDirection() const { return viewDirection; }
    float getHorizontalFOV() const { return horizontalFOV; }
};


class CameraBuilder {
private:
    Vec3 eyePosition;
    Vec3 upDirection;
    Vec3 viewDirection;
    float horizontalFOV;

public:
    // Setters
    void setEyePosition(const Vec3& eyePos) { this->eyePosition = eyePos; }
    void setUpDirection(const Vec3& upDir) { this->upDirection = upDir.normal(); }
    void setViewDirection(const Vec3& viewDir) { this->viewDirection = viewDir.normal(); }
    void setHorizontalFOV(float fov) { this->horizontalFOV = fov; }

    // Build method
    Camera build() { return Camera(eyePosition, upDirection, viewDirection, horizontalFOV); }
};
