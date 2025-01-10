#include "shapes.h"

#include <cmath>
#include <stdexcept>

// Determine if ray intersects sphere
bool Sphere::intersects(const Ray& ray, Vec3& intersectionPoint) const {
    Vec3 oc = ray.origin - this->position;
    double a = 1.0; //ray.getDirection().dot(ray.getDirection())
    double b = 2.0f * dot(oc, ray.getDirection());
    //double c = oc.dot(oc) - this->radius * this->radius;
    double c = pow(ray.origin.x - this->position.x, 2) + pow(ray.origin.y - this->position.y, 2) + pow(ray.origin.z - this->position.z, 2) - pow(this->radius, 2);
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
    intersectionPoint = ray.origin + ray.getDirection() * t;
    return true;
}


Vec2 Sphere::getTextureCoordinate(const Vec3& intersectionPoint) const {
    Vec3 localPoint = (intersectionPoint - position).normal();

    // Compute u and v texture coordinates
    double u = 0.5 + (atan2(localPoint.y, localPoint.x) / (2 * 3.1415927));
    double v = 0.5 - (asin(localPoint.z) / 3.1415927);

    // Rotate the u coordinate 180 degrees
    u = fmod(u + 0.5, 1.0); // Add 0.5 to rotate 180 degrees, then wrap around to [0, 1]

    return Vec2(u, v);
}

// calculate normal at intersectionPoint
Vec3 Sphere::getNormal(const Vec3& intersectionPoint) const {
    return (intersectionPoint - this->position).normal();
}


// Determine if ray intersects triangle
bool Triangle::intersects(const Ray& ray, Vec3& intersectionPoint) const {
    Vec3 edgeAB = this->vertexB - this->vertexA;
    Vec3 edgeAC = this->vertexC - this->vertexA;
    Vec3 normal = cross(edgeAB, edgeAC).normal();

    double discriminant = dot(normal, ray.getDirection());
    if (fabs(discriminant) < 1e-6f) return false;

    double D = dot(normal, this->vertexA);
    double ray_t = -(dot(normal, ray.origin) + D) / discriminant;
    if (ray_t < 1e-5f) return false;

    intersectionPoint = ray.origin + ray.getDirection() * ray_t;

    // Barycentric coordinates for the intersection point
    Vec3 v0 = this->vertexB - this->vertexA;
    Vec3 v1 = this->vertexC - this->vertexA;
    Vec3 v2 = intersectionPoint - this->vertexA;

    double dot00 = dot(v0, v0);
    double dot01 = dot(v0, v1);
    double dot02 = dot(v0, v2);
    double dot11 = dot(v1, v1);
    double dot12 = dot(v1, v2);

    double invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0 && v >= 0 && (u + v) <= 1);
}


//Vec2 Triangle::getTextureCoordinate(const Vec3& intersectionPoint) const {
//    Vec3 barycentricCoordinates = getBarycentricCoordinates(intersectionPoint);
//    double u = barycentricCoordinates.x * textureCoordinateA->x + barycentricCoordinates.y * textureCoordinateB->x + barycentricCoordinates.z * textureCoordinateC->x;
//    double v = barycentricCoordinates.x * textureCoordinateA->y + barycentricCoordinates.y * textureCoordinateB->y + barycentricCoordinates.z * textureCoordinateC->y;
//    return Vec2(u, v);
//}

Vec2 Triangle::getTextureCoordinate(const Vec3& intersectionPoint) const {
    if (!textureCoordinateA || !textureCoordinateB || !textureCoordinateC) {
        return Vec2(0, 0);
    }
    Vec3 barycentricCoordinates = getBarycentricCoordinates(intersectionPoint);
    double u = barycentricCoordinates.x * textureCoordinateA->x + barycentricCoordinates.y * textureCoordinateB->x + barycentricCoordinates.z * textureCoordinateC->x;
    double v = barycentricCoordinates.x * textureCoordinateA->y + barycentricCoordinates.y * textureCoordinateB->y + barycentricCoordinates.z * textureCoordinateC->y;
    return Vec2(u, v);
}



// calculate normal at intersectionPoint
Vec3 Triangle::getNormal(const Vec3& intersectionPoint) const {
    return (cross((vertexB - vertexA), vertexC - vertexA)).normal();
}


Vec3 Triangle::getBarycentricCoordinates(const Vec3& intersectionPoint) const {
    // Compute edges
    Vec3 e1 = vertexB - vertexA; // Edge 1: vertexB - vertexA
    Vec3 e2 = vertexC - vertexA; // Edge 2: vertexC - vertexA
    Vec3 ep = intersectionPoint - vertexA; // Vector from vertexA to intersectionPoint

    // Dot products
    double d11 = dot(e1, e1); // e1 • e1
    double d12 = dot(e1, e2); // e1 • e2
    double d22 = dot(e2, e2); // e2 • e2
    double dp1 = dot(ep, e1); // ep • e1
    double dp2 = dot(ep, e2); // ep • e2

    // Compute denominator
    double denom = d11 * d22 - d12 * d12;
    if (std::fabs(denom) < 1e-6) { // Add epsilon check for precision issues
        throw std::runtime_error("Error: Degenerate triangle with zero area.");
    }

    // Compute barycentric coordinates
    double beta = (d22 * dp1 - d12 * dp2) / denom;  // β = D_beta / denom
    double gamma = (d11 * dp2 - d12 * dp1) / denom; // γ = D_gamma / denom
    double alpha = 1.0 - beta - gamma;             // α = 1 - β - γ

    return Vec3(alpha, beta, gamma); // Return barycentric coordinates
}