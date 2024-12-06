#include "shapes.h"

#include <cmath>
#include <stdexcept>

// Determine if ray intersects sphere
bool Sphere::intersects(const Ray& ray, Vec3& intersectionPoint) const {
    Vec3 oc = ray.getOrigin() - this->position;
    double a = 1.0f; //ray.getDirection().dot(ray.getDirection())
    double b = 2.0f * oc.dot(ray.getDirection());
    //double c = oc.dot(oc) - this->radius * this->radius;
    double c = pow(ray.getOrigin().getX() - this->position.getX(), 2) + pow(ray.getOrigin().getY() - this->position.getY(), 2) + pow(ray.getOrigin().getZ() - this->position.getZ(), 2) - pow(this->radius, 2);
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
//Vec2 Sphere::getTextureCoordinate(const Vec3& intersectionPoint) const {
//    Vec3 localPoint = (intersectionPoint - position).normal();
//    double u = 0.5f + (atan2(localPoint.getY(), localPoint.getX()) / (2 * 3.1415927));
//    double v = 0.5f - (asin(localPoint.getZ()) / 3.1415927);
//    return Vec2(u, v);
//}

Vec2 Sphere::getTextureCoordinate(const Vec3& intersectionPoint) const {
    Vec3 localPoint = (intersectionPoint - position).normal();

    // Compute u and v texture coordinates
    double u = 0.5 + (atan2(localPoint.getY(), localPoint.getX()) / (2 * 3.1415927));
    double v = 0.5 - (asin(localPoint.getZ()) / 3.1415927);

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
    Vec3 normal = edgeAB.cross(edgeAC).normal();

    double discriminant = normal.dot(ray.getDirection());
    if (fabs(discriminant) < 1e-6f) return false;

    double D = -normal.dot(this->vertexA);
    double ray_t = -(normal.dot(ray.getOrigin()) + D) / discriminant;
    if (ray_t < 1e-5f) return false;

    intersectionPoint = ray.getOrigin() + ray.getDirection() * ray_t;

    // Barycentric coordinates for the intersection point
    Vec3 v0 = this->vertexB - this->vertexA;
    Vec3 v1 = this->vertexC - this->vertexA;
    Vec3 v2 = intersectionPoint - this->vertexA;

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
        std::min({ vertexA.getX(), vertexB.getX(), vertexC.getX() }),
        std::min({ vertexA.getY(), vertexB.getY(), vertexC.getY() }),
        std::min({ vertexA.getZ(), vertexB.getZ(), vertexC.getZ() })
    );
}

Vec3 Triangle::getBoundingBoxMax() const {
    return Vec3(
        std::max({ vertexA.getX(), vertexB.getX(), vertexC.getX() }),
        std::max({ vertexA.getY(), vertexB.getY(), vertexC.getY() }),
        std::max({ vertexA.getZ(), vertexB.getZ(), vertexC.getZ() })
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
    //if (normalMap && vertexANormal && vertexBNormal && vertexCNormal) {
    //    // Retrieve the interpolated texture coordinates using the helper function
    //    Vec2 uv = getTextureCoordinate(intersectionPoint);
    //    double u = uv.getX();
    //    double v = uv.getY();

    //    // Fetch the normal from the normal map
    //    Vec3 m = normalMap->getNormal(u, v);

    //    // Interpolate the base normal
    //    Vec3 barycentric = getBarycentricCoordinates(intersectionPoint);
    //    Vec3 N = (*vertexANormal * barycentric.getX() +
    //        *vertexBNormal * barycentric.getY() +
    //        *vertexCNormal * barycentric.getZ())
    //        .normal();

    //    // Compute tangents and bitangents
    //    Vec3 e1 = vertexB - vertexA;
    //    Vec3 e2 = vertexC - vertexA;

    //    double u1 = textureCoordinateB->getX() - textureCoordinateA->getX();
    //    double v1 = textureCoordinateB->getY() - textureCoordinateA->getY();
    //    double u2 = textureCoordinateC->getX() - textureCoordinateA->getX();
    //    double v2 = textureCoordinateC->getY() - textureCoordinateA->getY();

    //    double invDet = 1.0 / (u1 * v2 - u2 * v1);
    //    Vec3 T = (e1 * v2 - e2 * v1) * invDet;
    //    Vec3 B = (e2 * u1 - e1 * u2) * invDet;

    //    // Normalize T and B
    //    T = T.normal();
    //    B = B.normal();

    //    // Transform normal from tangent space to world space
    //    Vec3 newN = (T * m.getX() + B * m.getY() + N * m.getZ()).normal();
    //    return newN;
    //}
    //if (vertexANormal && vertexBNormal && vertexCNormal) {
    //    Vec3 barycentric = getBarycentricCoordinates(intersectionPoint);
    //    return (*vertexANormal * barycentric.getX() + *vertexBNormal * barycentric.getY() + *vertexCNormal * barycentric.getZ()).normal();
    //}
    return ((vertexB - vertexA).cross(vertexC - vertexA)).normal();
}


Vec3 Triangle::getBarycentricCoordinates(const Vec3& intersectionPoint) const {
    // Compute edges
    Vec3 e1 = vertexB - vertexA; // Edge 1: vertexB - vertexA
    Vec3 e2 = vertexC - vertexA; // Edge 2: vertexC - vertexA
    Vec3 ep = intersectionPoint - vertexA; // Vector from vertexA to intersectionPoint

    // Dot products
    double d11 = e1.dot(e1); // e1 • e1
    double d12 = e1.dot(e2); // e1 • e2
    double d22 = e2.dot(e2); // e2 • e2
    double dp1 = ep.dot(e1); // ep • e1
    double dp2 = ep.dot(e2); // ep • e2

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
