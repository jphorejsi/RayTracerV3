#pragma once

#include "vec.h"
#include <stdexcept>

class Ray {
private:
    Vec3 direction; // must be greater than 0

public:
    Vec3 origin; // can be any value

    // Constructor
    Ray(Vec3 origin, Vec3 direction) {
        if (direction.length() == 0.0) {
            throw std::invalid_argument("Direction magnitude must be greater than 0.");
        }
        this->direction = direction.normal();
        this->origin = origin;
    }

    // Getters
    Vec3 getDirection() const { return this->direction; }

    // Setters
    void setDirection(const Vec3& direction) {
        if (direction.length() == 0.0) {
            throw std::invalid_argument("Direction magnitude must be greater than 0.");
        }
        this->direction = direction.normal();
    }
};
