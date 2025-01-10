#pragma once

#include "vec.h"

class Ray {
private:
    Vec3 direction; // must be greater than 0

public:
    Vec3 origin; // can be any value

    // Constructor
    Ray(Vec3 origin, Vec3 direction);

    // Getters
    Vec3 getDirection() const { return direction; }

    // Setters
    void setDirection(const Vec3& direction);
};
