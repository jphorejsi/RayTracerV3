#include "shapes.h"

#include <cmath>
#include <stdexcept>

// Determine if ray intersects sphere
bool Sphere::intersects(const Ray& ray, Vec3& intersectionPoint) const {
    Vec3 oc = ray.getOrigin() - this->position;
    float a = ray.getDirection().dot(ray.getDirection());
    float b = 2.0f * oc.dot(ray.getDirection());
    float c = oc.dot(oc) - this->radius * this->radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false;

    float sqrtDiscriminant = sqrt(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2.0f * a);
    float t2 = (-b + sqrtDiscriminant) / (2.0f * a);

    float min = 0.0001f;
    float max = FLT_MAX;

    float t = (t1 >= min && t1 <= max) ? t1 : (t2 >= min && t2 <= max) ? t2 : FLT_MAX;
    if (t == FLT_MAX) return false;

    intersectionPoint = ray.getOrigin() + ray.getDirection() * t;
    return true;
}

// Sphere bounding box implementation
Vec3 Sphere::getBoundingBoxMin() const {
    return this->position - Vec3(this->radius, this->radius, this->radius);
}

Vec3 Sphere::getBoundingBoxMax() const {
    return this->position + Vec3(this->radius, this->radius, this->radius);
}

// calcuate Sphere TC at intersectionPoint
Vec2 Sphere::calculateTextureCoordinate(const Vec3& intersectionPoint) const {
    Vec3 localPoint = (intersectionPoint - position).normal();
    float u = 0.5f + (atan2(localPoint.getZ(), localPoint.getX()) / (2 * 3.1415927));
    float v = 0.5f - (asin(localPoint.getY()) / 3.1415927);
    return Vec2(u, v);
}

// Determine if ray intersects triangle
bool Triangle::intersects(const Ray& ray, Vec3& intersectionPoint) const {
    Vec3 edgeAB = *this->vertexB - *this->vertexA;
    Vec3 edgeAC = *this->vertexC - *this->vertexA;
    Vec3 normal = edgeAB.cross(edgeAC).normal();

    float discriminant = normal.dot(ray.getDirection());
    if (fabs(discriminant) < 1e-6f) return false;

    float D = -normal.dot(*this->vertexA);
    float ray_t = -(normal.dot(ray.getOrigin()) + D) / discriminant;
    if (ray_t < 1e-5f) return false;

    intersectionPoint = ray.getOrigin() + ray.getDirection() * ray_t;

    // Barycentric coordinates for the intersection point
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

// Triangle bounding box implementation
Vec3 Triangle::getBoundingBoxMin() const {
    return Vec3(
        std::min({ this->vertexA->getX(), this->vertexB->getX(), this->vertexC->getX() }),
        std::min({ this->vertexA->getY(), this->vertexB->getY(), this->vertexC->getY() }),
        std::min({ this->vertexA->getZ(), this->vertexB->getZ(), this->vertexC->getZ() })
    );
}

Vec3 Triangle::getBoundingBoxMax() const {
    return Vec3(
        std::max({ this->vertexA->getX(), this->vertexB->getX(), this->vertexC->getX() }),
        std::max({ this->vertexA->getY(), this->vertexB->getY(), this->vertexC->getY() }),
        std::max({ this->vertexA->getZ(), this->vertexB->getZ(), this->vertexC->getZ() })
    );
}


Vec2 Triangle::calculateTextureCoordinate(const Vec3& intersectionPoint) const {
    Vec3 barycentricCoordinates = calculateBarycentricCoordinates(intersectionPoint);
    float u = barycentricCoordinates.getX() * textureCoordinateA->getX() + barycentricCoordinates.getY() * textureCoordinateB->getX() + barycentricCoordinates.getZ() * textureCoordinateC->getX();
    float v = barycentricCoordinates.getX() * textureCoordinateA->getY() + barycentricCoordinates.getY() * textureCoordinateB->getY() + barycentricCoordinates.getZ() * textureCoordinateC->getY();
    return Vec2(u, v);
}


Vec3 Triangle::calculateBarycentricCoordinates(const Vec3& intersectionPoint) const {
    Vec3 v0 = vertexB - vertexA;
    Vec3 v1 = vertexC - vertexA;
    Vec3 v2 = intersectionPoint - *vertexA;
    float d00 = v0.dot(v0);
    float d01 = v0.dot(v1);
    float d11 = v1.dot(v1);
    float d20 = v2.dot(v0);
    float d21 = v2.dot(v1);
    // Calculate the denominator
    float denom = d00 * d11 - d01 * d01;
    if (denom == 0) {
        throw std::runtime_error("Error: Degenerate triangle with zero area.");
    }
    // Calculate barycentric coordinates
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;
    return Vec3(u, v, w);
}
