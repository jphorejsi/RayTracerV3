#pragma once
#include "vec.h"

class CameraType {
private:
    Vec3 eyePosition;
    Vec3 upDirection;
    Vec3 viewDirection;
    float horizontalFOV;

public:
    // Constructor
    CameraType() = default;
    CameraType(const Vec3& eyePosition, const Vec3& upDirection, const Vec3& viewDirection, float horizontalFOV) : eyePosition(eyePosition), upDirection(upDirection), viewDirection(viewDirection), horizontalFOV(horizontalFOV) {}

    // Getters
    Vec3 getEyePosition() const { return eyePosition; }
    Vec3 getUpDirection() const { return upDirection; }
    Vec3 getViewDirection() const { return viewDirection; }
    float getHorizontalFOV() const { return horizontalFOV; }

    // Setters
    void setEyePosition(const Vec3& eyePosition) { this->eyePosition = eyePosition; }
    void setUpDirection(const Vec3& upDirection) { this->upDirection = upDirection; }
    void setViewDirection(const Vec3& viewDirection) { this->viewDirection = viewDirection; }
    void setHorizontalFOV(float horizontalFOV) { this->horizontalFOV = horizontalFOV; }

    // Other methods
};
