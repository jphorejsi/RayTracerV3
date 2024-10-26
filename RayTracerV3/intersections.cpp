#include "shapes.h"
#include <algorithm>  // For std::min and std::max

bool AABB::intersectsAABB(const Ray& ray) const {
    float tMin = (this->min.getX() - ray.getOrigin().getX()) / ray.getDirection().getX();
    float tMax = (this->max.getX() - ray.getOrigin().getX()) / ray.getDirection().getX();

    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (this->min.getY() - ray.getOrigin().getY()) / ray.getDirection().getY();
    float tyMax = (this->max.getY() - ray.getOrigin().getY()) / ray.getDirection().getY();

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax)) return false;

    tMin = std::max(tMin, tyMin);
    tMax = std::min(tMax, tyMax);

    float tzMin = (this->min.getZ() - ray.getOrigin().getZ()) / ray.getDirection().getZ();
    float tzMax = (this->max.getZ() - ray.getOrigin().getZ()) / ray.getDirection().getZ();

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax)) return false;

    return true;
}

bool SphereType::intersects(const Ray& ray) const {
    Vec3 rayCenter = ray.getOrigin();
    Vec3 sphereCenter = this->getBoundingBoxMin() + (this->getBoundingBoxMax() - this->getBoundingBoxMin()) / 2;
    Vec3 dir = ray.getDirection();

    float B = 2 * (dir.getX() * (rayCenter.getX() - sphereCenter.getX()) +
        dir.getY() * (rayCenter.getY() - sphereCenter.getY()) +
        dir.getZ() * (rayCenter.getZ() - sphereCenter.getZ()));

    float C = pow(rayCenter.getX() - sphereCenter.getX(), 2) +
        pow(rayCenter.getY() - sphereCenter.getY(), 2) +
        pow(rayCenter.getZ() - sphereCenter.getZ(), 2) -
        pow(this->radius, 2);

    float discriminant = pow(B, 2) - 4 * C;

    if (discriminant < 0) {
        return false;
    }

    float t_1 = (-B + sqrt(discriminant)) / 2;
    float t_2 = (-B - sqrt(discriminant)) / 2;

    if (t_1 > t_2) {
        std::swap(t_1, t_2);
    }

    float min = 0.0001f;
    float max = FLT_MAX;

    return (t_1 >= min && t_1 <= max) || (t_2 >= min && t_2 <= max);
}

bool TriangleType::intersects(const Ray& ray) const {
    Vec3 edgeAB = *this->vertexB - *this->vertexA;
    Vec3 edgeAC = *this->vertexC - *this->vertexA;

    Vec3 normal = (edgeAB.cross(edgeAC)).normal();

    float discriminant = normal.dot(ray.getDirection());
    if (std::abs(discriminant) < 1e-6) {
        return false;
    }

    float D = -normal.dot(*this->vertexA);
    float ray_t = -(normal.dot(ray.getOrigin()) + D) / discriminant;

    if (ray_t < 1e-5) {
        return false;
    }

    Vec3 intersectionPoint = ray.getOrigin() + ray.getDirection() * ray_t;

    Vec3 v0 = *this->vertexB - *this->vertexA;
    Vec3 v1 = *this->vertexC - *this->vertexA;
    Vec3 v2 = intersectionPoint - *this->vertexA;

    float dot00 = v0.dot(v0);
    float dot01 = v0.dot(v1);
    float dot02 = v0.dot(v2);
    float dot11 = v1.dot(v1);
    float dot12 = v1.dot(v2);

    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0 && v >= 0 && (u + v) <= 1);
}
