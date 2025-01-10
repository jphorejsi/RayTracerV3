#include "DepthCue.h"

#include<stdexcept>

DepthCue::DepthCue(const Color& depthCueColor, double alphaMax, double alphaMin, double distanceMax, double distanceMin) {
    if (depthCueColor.getR() < 0.0 || depthCueColor.getR() > 1.0 ||
        depthCueColor.getG() < 0.0 || depthCueColor.getG() > 1.0 ||
        depthCueColor.getB() < 0.0 || depthCueColor.getB() > 1.0) {
        throw std::invalid_argument("Depth cue color values must be between 0 and 1");
    }
    if (alphaMax < 0.0 || alphaMax > 1.0) {
        throw std::invalid_argument("AlphaMax must be between 0 and 1");
    }
    if (alphaMin < 0.0 || alphaMin > 1.0) {
        throw std::invalid_argument("AlphaMin must be between 0 and 1");
    }
    if (distanceMax <= distanceMin) {
        throw std::invalid_argument("DistanceMax must be greater than DistanceMin");
    }
    this->depthCueColor = depthCueColor;
    this->alphaMax = alphaMax;
    this->alphaMin = alphaMin;
    this->distanceMax = distanceMax;
    this->distanceMin = distanceMin;
}

void DepthCue::setDepthCueColor(const Color& depthCueColor) {
    if (depthCueColor.getR() < 0.0 || depthCueColor.getR() > 1.0 ||
        depthCueColor.getG() < 0.0 || depthCueColor.getG() > 1.0 ||
        depthCueColor.getB() < 0.0 || depthCueColor.getB() > 1.0) {
        throw std::invalid_argument("Depth cue color values must be between 0 and 1");
    }
    this->depthCueColor = depthCueColor;
}

void DepthCue::setAlphaMax(double alphaMax) {
    if (alphaMax < 0.0 || alphaMax > 1.0) {
        throw std::invalid_argument("AlphaMax must be between 0 and 1");
    }
    this->alphaMax = alphaMax;
}

void DepthCue::setAlphaMin(double alphaMin) {
    if (alphaMin < 0.0 || alphaMin > 1.0) {
        throw std::invalid_argument("AlphaMin must be between 0 and 1");
    }
    this->alphaMin = alphaMin;
}

void DepthCue::setDistanceMax(double distanceMax) {
    if (distanceMax <= this->distanceMin) {
        throw std::invalid_argument("DistanceMax must be greater than DistanceMin");
    }
    this->distanceMax = distanceMax;
}

void DepthCue::setDistanceMin(double distanceMin) {
    if (distanceMin >= this->distanceMax) {
        throw std::invalid_argument("DistanceMin must be less than DistanceMax");
    }
    this->distanceMin = distanceMin;
}

double DepthCue::calculateDepthCueingFactor(double distance) const {
    if (distance <= distanceMin) {
        return alphaMax;
    }
    else if (distance >= distanceMax) {
        return alphaMin;
    }
    else {
        return alphaMin + (alphaMax - alphaMin) * (distanceMax - distance) / (distanceMax - distanceMin);
    }
}
