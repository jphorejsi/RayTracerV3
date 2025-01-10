#pragma once
#include "vec.h"
#include "ray.h"
#include <limits>

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


    // Other methods
    void expand(const AABB& other) {
        minBounds = Vec3::min(this->minBounds, other.minBounds);
        maxBounds = Vec3::max(maxBounds, other.maxBounds);
    }

    double surfaceArea() const {
        Vec3 size = maxBounds - minBounds;
        return 2.0f * (size.x * size.y + size.y * size.z + size.z * size.x);
    }

    bool intersects(const Ray& ray) const;



    double getIntersectionDistance(const Ray& ray) const {
        double invDirX = (ray.getDirection().x != 0.0) ? 1.0 / ray.getDirection().x : std::numeric_limits<double>::infinity();
        double invDirY = (ray.getDirection().y != 0.0) ? 1.0 / ray.getDirection().y : std::numeric_limits<double>::infinity();
        double invDirZ = (ray.getDirection().z != 0.0) ? 1.0 / ray.getDirection().z : std::numeric_limits<double>::infinity();

        double tMin = (this->minBounds.x - ray.origin.x) * invDirX;
        double tMax = (this->maxBounds.x - ray.origin.x) * invDirX;
        if (tMin > tMax) std::swap(tMin, tMax);

        double tyMin = (this->minBounds.y - ray.origin.y) * invDirY;
        double tyMax = (this->maxBounds.y - ray.origin.y) * invDirY;
        if (tyMin > tyMax) std::swap(tyMin, tyMax);

        if ((tMin > tyMax) || (tyMin > tMax)) {
            return std::numeric_limits<double>::infinity(); // No intersection
        }

        if (tyMin > tMin) tMin = tyMin;
        if (tyMax < tMax) tMax = tyMax;

        double tzMin = (this->minBounds.z - ray.origin.z) * invDirZ;
        double tzMax = (this->maxBounds.z - ray.origin.z) * invDirZ;
        if (tzMin > tzMax) std::swap(tzMin, tzMax);

        if ((tMin > tzMax) || (tzMin > tMax)) {
            return std::numeric_limits<double>::infinity(); // No intersection
        }

        if (tzMin > tMin) tMin = tzMin;
        if (tzMax < tMax) tMax = tzMax;

        // Return the first intersection distance along the ray
        return (tMin > 0) ? tMin : tMax;
    }

};