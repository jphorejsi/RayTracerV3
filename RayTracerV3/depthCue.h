#pragma once
#include "color.h"


class DepthCue {
private:
    Color depthCueColor;
    double alphaMax;
    double alphaMin;
    double distanceMax;
    double distanceMin;

public:
    // Constructor
    DepthCue(const Color& depthCue, double alphaMax, double alphaMin, double distanceMax, double distanceMin);

    // Getters
    Color getDepthCueColor() const { return depthCueColor; }
    double getAlphaMax() const { return alphaMax; }
    double getAlphaMin() const { return alphaMin; }
    double getDistanceMax() const { return distanceMax; }
    double getDistanceMin() const { return distanceMin; }

    // Setters
    void setDepthCueColor(const Color& depthCueColor);
    void setAlphaMax(double alphaMax);
    void setAlphaMin(double alphaMin);
    void setDistanceMax(double distanceMax);
    void setDistanceMin(double distanceMin);

    // Other methods
    double calculateDepthCueingFactor(double distance) const;
};
