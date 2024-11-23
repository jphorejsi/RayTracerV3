#include "shapes.h"

#include <cmath>
#include <stdexcept>

// Determine if ray intersects sphere
bool Sphere::intersects(const Ray& ray, Vec3& intersectionPoint) const {
    Vec3 oc = ray.getOrigin() - this->position;
    double a = 1.0f; //ray.getDirection().dot(ray.getDirection())
    double b = 2.0f * oc.dot(ray.getDirection());
    double c = oc.dot(oc) - this->radius * this->radius;
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false;

    double t1 = (-b - sqrt(discriminant)) / (2.0f * a);
    double t2 = (-b + sqrt(discriminant)) / (2.0f * a);

    double min = 1e-6;
    double max = FLT_MAX;

    // Check if both roots are invalid
    if ((t1 < min || t1 > max) && (t2 < min || t2 > max)) {
        return false; // No valid intersections
    }

    // Choose the closest valid intersection
    double t = FLT_MAX;
    if (t1 >= min && t1 <= max) {
        t = t1;
    } else if (t2 >= min && t2 <= max) {
        t = t2;
    }

    // Ensure we have a valid t value
    if (t == FLT_MAX) return false;

    // Compute the intersection point
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

// calculate Sphere TC at intersectionPoint
Vec2 Sphere::getTextureCoordinate(const Vec3& intersectionPoint) const {
    Vec3 localPoint = (intersectionPoint - position).normal();
    double u = 0.5f + (atan2(localPoint.getZ(), localPoint.getX()) / (2 * 3.1415927));
    double v = 0.5f - (asin(localPoint.getY()) / 3.1415927);
    return Vec2(u, v);
}

// calculate normal at intersectionPoint
Vec3 Sphere::getNormal(const Vec3& intersectionPoint) const {
    // add if for normalMap
    return (intersectionPoint - this->position).normal();
}

// Determine if ray intersects triangle
bool Triangle::intersects(const Ray& ray, Vec3& intersectionPoint) const {
    Vec3 edgeAB = *this->vertexB - *this->vertexA;
    Vec3 edgeAC = *this->vertexC - *this->vertexA;
    Vec3 normal = edgeAB.cross(edgeAC).normal();

    double discriminant = normal.dot(ray.getDirection());
    if (fabs(discriminant) < 1e-6f) return false;

    double D = -normal.dot(*this->vertexA);
    double ray_t = -(normal.dot(ray.getOrigin()) + D) / discriminant;
    if (ray_t < 1e-5f) return false;

    intersectionPoint = ray.getOrigin() + ray.getDirection() * ray_t;

    // Barycentric coordinates for the intersection point
    Vec3 v0 = *this->vertexB - *this->vertexA;
    Vec3 v1 = *this->vertexC - *this->vertexA;
    Vec3 v2 = intersectionPoint - *this->vertexA;

    double dot00 = v0.dot(v0);
    double dot01 = v0.dot(v1);
    double dot02 = v0.dot(v2);
    double dot11 = v1.dot(v1);
    double dot12 = v1.dot(v2);

    double invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

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


Vec2 Triangle::getTextureCoordinate(const Vec3& intersectionPoint) const {
    Vec3 barycentricCoordinates = getBarycentricCoordinates(intersectionPoint);
    double u = barycentricCoordinates.getX() * textureCoordinateA->getX() + barycentricCoordinates.getY() * textureCoordinateB->getX() + barycentricCoordinates.getZ() * textureCoordinateC->getX();
    double v = barycentricCoordinates.getX() * textureCoordinateA->getY() + barycentricCoordinates.getY() * textureCoordinateB->getY() + barycentricCoordinates.getZ() * textureCoordinateC->getY();
    return Vec2(u, v);
}

// calculate normal at intersectionPoint
Vec3 Triangle::getNormal(const Vec3& intersectionPoint) const {
    // add if for normalMap
    if (vertexANormal && vertexBNormal && vertexCNormal) {
        Vec3 barycentric = getBarycentricCoordinates(intersectionPoint);
        return (*vertexANormal * barycentric.getX() + *vertexBNormal * barycentric.getY() + *vertexCNormal * barycentric.getZ()).normal();
    }
    return ((*vertexB - *vertexA).cross(*vertexC - *vertexA)).normal();
}


Vec3 Triangle::getBarycentricCoordinates(const Vec3& intersectionPoint) const {
    Vec3 v0 = vertexB - vertexA;
    Vec3 v1 = vertexC - vertexA;
    Vec3 v2 = intersectionPoint - *vertexA;
    double d00 = v0.dot(v0);
    double d01 = v0.dot(v1);
    double d11 = v1.dot(v1);
    double d20 = v2.dot(v0);
    double d21 = v2.dot(v1);
    // get the denominator
    double denom = d00 * d11 - d01 * d01;
    if (denom == 0) {
        throw std::runtime_error("Error: Degenerate triangle with zero area.");
    }
    // get barycentric coordinates
    double v = (d11 * d20 - d01 * d21) / denom;
    double w = (d00 * d21 - d01 * d20) / denom;
    double u = 1.0f - v - w;
    return Vec3(u, v, w);
}
