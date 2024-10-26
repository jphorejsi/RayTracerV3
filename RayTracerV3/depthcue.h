#pragma once
#include "color.h"

class DepthCueType {
private:
    ColorType dc;              // Depth cue color
    float alphaMax;       // Maximum alpha value
    float alphaMin;       // Minimum alpha value
    float distanceMax;    // Maximum distance for depth cueing
    float distanceMin;    // Minimum distance for depth cueing
    bool enabled;         // Whether depth cueing is enabled

public:
    // Constructor
    DepthCueType(ColorType dc, float alphaMax = 0, float alphaMin = 0, float distanceMax = 0, float distanceMin = 0, bool enabled = false) : dc(dc), alphaMax(alphaMax), alphaMin(alphaMin), distanceMax(distanceMax), distanceMin(distanceMin), enabled(enabled) {}

};
