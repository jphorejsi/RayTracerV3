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
    // Setter methods in a one-line format
    CameraBuilder& setEyePosition(const Vec3& eyePos) { this->eyePosition = eyePos; return *this; }
    CameraBuilder& setUpDirection(const Vec3& upDir) { this->upDirection = upDir; return *this; }
    CameraBuilder& setViewDirection(const Vec3& viewDir) { this->viewDirection = viewDir; return *this; }
    CameraBuilder& setHorizontalFOV(float fov) { this->horizontalFOV = fov; return *this; }

    // Final build method to create a Camera object
    Camera build() { return Camera(eyePosition, upDirection, viewDirection, horizontalFOV); }
};
