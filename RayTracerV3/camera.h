#pragma once
#include "vec.h"

class Camera {
private:
    Vec3 eyePosition;
    Vec3 upDirection;
    Vec3 viewDirection;
    double horizontalFOV;

public:
    // Constructors
    Camera() = default;
    Camera(const Vec3& eyePosition, const Vec3& upDirection, const Vec3& viewDirection, double horizontalFOV);

    // Getters
    Vec3 getEyePosition() const { return eyePosition; }
    Vec3 getUpDirection() const { return upDirection; }
    Vec3 getViewDirection() const { return viewDirection; }
    double getHorizontalFOV() const { return horizontalFOV; }

    void setEyePosition(const Vec3& eyePos);
    void setUpDirection(const Vec3& upDir);
    void setViewDirection(const Vec3& viewDir);
    void setHorizontalFOV(double fov);
};
