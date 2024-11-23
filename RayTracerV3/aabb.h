#pragma once
#include "vec.h"

class AABB {
private:
    Vec3 minBounds;
    Vec3 maxBounds;

public:
    // Constructor
    AABB() : minBounds(std::numeric_limits<double>::max()), maxBounds(std::numeric_limits<double>::lowest()) {}
    AABB(const Vec3& minBounds, const Vec3& maxBounds) : minBounds(minBounds), maxBounds(maxBounds) {}

    // Getters
    Vec3 getMinBounds() const { return this->minBounds; }
    Vec3 getMaxBounds() const { return this->maxBounds; }

    // Setters
    void setBoundingBoxMin(const Vec3& minBounds) { this->minBounds = minBounds; }
    void setBoundingBoxMax(const Vec3& maxBounds) { this->maxBounds = maxBounds; }

    // Other methods
    void expand(const AABB& other) {
        minBounds = Vec3::min(this->minBounds, other.minBounds);
        maxBounds = Vec3::max(maxBounds, other.maxBounds);
    }

    double surfaceArea() const {
        Vec3 size = maxBounds - minBounds;
        return 2.0f * (size.getX() * size.getY() + size.getY() * size.getZ() + size.getZ() * size.getX());
    }

    bool intersects(const Ray& ray) const;

    AABB& operator=(const AABB& other) {
        if (this != &other) {
            // Copy fields from other to this
            this->minBounds = other.minBounds;
            this->maxBounds = other.maxBounds;
        }
        return *this;
    }
};