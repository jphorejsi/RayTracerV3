#include "DepthCue.h"

#include<stdexcept>

DepthCue::DepthCue(const Color& depthCue, double alphaMax, double alphaMin, double distanceMax, double distanceMin) {
    if (depthCue.getR() < 0.0 || depthCue.getR() > 1.0 ||
        depthCue.getG() < 0.0 || depthCue.getG() > 1.0 ||
        depthCue.getB() < 0.0 || depthCue.getB() > 1.0) {
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
    this->depthCue = depthCue;
    this->alphaMax = alphaMax;
    this->alphaMin = alphaMin;
    this->distanceMax = distanceMax;
    this->distanceMin = distanceMin;
}

void DepthCue::setDepthCue(const Color& depthCue) {
    if (depthCue.getR() < 0.0 || depthCue.getR() > 1.0 ||
        depthCue.getG() < 0.0 || depthCue.getG() > 1.0 ||
        depthCue.getB() < 0.0 || depthCue.getB() > 1.0) {
        throw std::invalid_argument("Depth cue color values must be between 0 and 1");
    }
    this->depthCue = depthCue;
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
