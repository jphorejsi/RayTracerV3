#pragma once
#include "vec.h"

class Ray {
private:
    Vec3 origin, direction;

public:
    // Constructor
    Ray(Vec3 origin = Vec3(), Vec3 direction = Vec3()) : origin(origin), direction(direction) {}

    // Getters
    Vec3 getOrigin() const { return origin; }
    Vec3 getDirection() const { return direction; }

    // Setters
    void setOrigin(const Vec3& origin) { this->origin = origin; }
    void setDirection(const Vec3& direction) { this->direction = direction; }

    // Other methods
    //ColorType traceRay(const SceneType& scene);
};
