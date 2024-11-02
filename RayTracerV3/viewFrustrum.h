#pragma once
#include "vec.h"
#include "camera.h"
#include "imageSize.h"

class ViewFrustrum {
private:
    float width, height;
    Vec3 upperLeft, upperRight, lowerLeft, lowerRight;

public:
    // Constructor
    ViewFrustrum() = default;
    ViewFrustrum(const Camera& camera, const ImageSize& imageSize);

    // Getters
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    Vec3 getUpperLeft() const { return upperLeft; }
    Vec3 getUpperRight() const { return upperRight; }
    Vec3 getLowerLeft() const { return lowerLeft; }
    Vec3 getLowerRight() const { return lowerRight; }

    // Setters
    void setWidth(float w) { width = w; }
    void setHeight(float h) { height = h; }
    void setUpperLeft(const Vec3& vec) { upperLeft = vec; }
    void setUpperRight(const Vec3& vec) { upperRight = vec; }
    void setLowerLeft(const Vec3& vec) { lowerLeft = vec; }
    void setLowerRight(const Vec3& vec) { lowerRight = vec; }

    // Other methods
};
