#pragma once
#include "vec.h"

class Camera {
private:
    // Camera variables
    Vec3 upDirection;
    Vec3 viewDirection;
    double horizontalFOV;

    // View frustum variables
    double VFWidth = 0.0;
    double VFHeight = 0.0;
    Vec3 VFUpperLeft, VFUpperRight, VFLowerLeft, VFLowerRight;

public:
    Vec3 eyePosition;

    // Constructors
    Camera(const Vec3& eyePosition, const Vec3& upDirection, const Vec3& viewDirection, double horizontalFOV);

    // Getters for Camera Variables
    Vec3 getUpDirection() const { return upDirection; }
    Vec3 getViewDirection() const { return viewDirection; }
    double getHorizontalFOV() const { return horizontalFOV; }

    // Setters for Camera Variables
    void setEyePosition(const Vec3 eyePos) { eyePosition = eyePos; }
    void setUpDirection(const Vec3 upDir);
    void setViewDirection(const Vec3 viewDir);
    void setHorizontalFOV(double fov);

    // Getters for View Frustum Variables
    double getVFWidth() const { return VFWidth; }
    double getVFHeight() const { return VFHeight; }

    const Vec3& getVFUpperLeft() const { return VFUpperLeft; }
    const Vec3& getVFUpperRight() const { return VFUpperRight; }
    const Vec3& getVFLowerLeft() const { return VFLowerLeft; }
    const Vec3& getVFLowerRight() const { return VFLowerRight; }

    // Other methods
    void createFrustrum(int imageWidth, int imageHeight);
};

