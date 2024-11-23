#pragma once
#include "vec.h"
#include "camera.h"
#include "imageSize.h"

class ViewFrustrum {
private:
    double width, height;
    Vec3 upperLeft, upperRight, lowerLeft, lowerRight;

public:
    // Constructor
    ViewFrustrum() = default;
    ViewFrustrum(const Camera& camera, const ImageSize& imageSize);

    // Getters
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    Vec3 getUpperLeft() const { return upperLeft; }
    Vec3 getUpperRight() const { return upperRight; }
    Vec3 getLowerLeft() const { return lowerLeft; }
    Vec3 getLowerRight() const { return lowerRight; }
};
