#include "shapes.h"
#include <cmath>  // For sqrt and pow
#include <stdexcept>

// Deternube if ray intersects sphere
bool Sphere::intersects(const Ray& ray) const {
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

    return (t1 >= min && t1 <= max) || (t2 >= min && t2 <= max);
}

// Sphere bounding box implementation
Vec3 Sphere::getBoundingBoxMin() const {
    return this->position - Vec3(this->radius, this->radius, this->radius);
}

Vec3 Sphere::getBoundingBoxMax() const {
    return this->position + Vec3(this->radius, this->radius, this->radius);
}

// Determine if ray intersects triangle
bool Triangle::intersects(const Ray& ray) const {
    Vec3 edgeAB = *this->vertexB - *this->vertexA;
    Vec3 edgeAC = *this->vertexC - *this->vertexA;
    Vec3 normal = edgeAB.cross(edgeAC).normal();

    float discriminant = normal.dot(ray.getDirection());
    if (fabs(discriminant) < 1e-6f) return false;

    float D = -normal.dot(*this->vertexA);
    float ray_t = -(normal.dot(ray.getOrigin()) + D) / discriminant;
    if (ray_t < 1e-5f) return false;

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

// SphereFactory method implementations
IShape* SphereFactory::createShape(const Vec3& position, float radius, IMaterial* material) {
    if (radius <= 0) {
        throw std::runtime_error("Error: Radius must be positive.");
    }
    Sphere* sphere = new Sphere(position, radius);  // Initialize with only position and radius
    sphere->setMaterial(material ? material : new RGBMaterial(Color(1, 1, 1)));  // Set material, defaulting to white if null
    return sphere;
}

IShape* SphereFactory::createTexturedShape(const Vec3& position, float radius, IMaterial* material, Texture* texture) {
    if (radius <= 0) {
        throw std::runtime_error("Error: Radius must be positive.");
    }
    if (!texture) {
        throw std::runtime_error("Error: Texture must be provided for a textured shape.");
    }
    Sphere* sphere = new Sphere(position, radius);  // Initialize with only position and radius
    sphere->setMaterial(material ? material : new RGBMaterial(Color(1, 1, 1)));
    sphere->setTexture(texture);  // Set texture after construction
    return sphere;
}

IShape* SphereFactory::createNormalMappedShape(const Vec3& position, float radius, IMaterial* material, Texture* texture, NormalMap* normalMap) {
    if (radius <= 0) {
        throw std::runtime_error("Error: Radius must be positive.");
    }
    if (!texture) {
        throw std::runtime_error("Error: Texture must be provided for a normal mapped shape.");
    }
    if (!normalMap) {
        throw std::runtime_error("Error: Normal map must be provided for a normal mapped shape.");
    }
    Sphere* sphere = new Sphere(position, radius);  // Initialize with only position and radius
    sphere->setMaterial(material ? material : new RGBMaterial(Color(1, 1, 1)));
    sphere->setTexture(texture);
    sphere->setNormalMap(normalMap);  // Set normal map after construction
    return sphere;
}

// TriangleFactory method implementations
IShape* TriangleFactory::createShape(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, IMaterial* material) {
    if (!material) {
        material = new RGBMaterial(Color(1, 1, 1));  // Default to white if no material provided
    }
    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);  // Initialize vertices
    triangle->setMaterial(material);
    return triangle;
}

IShape* TriangleFactory::createTexturedShape(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, IMaterial* material, Texture* texture) {
    if (!material) {
        material = new RGBMaterial(Color(1, 1, 1));
    }
    if (!texture) {
        throw std::runtime_error("Error: Texture must be provided for a textured shape.");
    }
    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
    triangle->setMaterial(material);
    triangle->setTexture(texture);
    return triangle;
}

IShape* TriangleFactory::createNormalMappedShape(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, IMaterial* material, Texture* texture, NormalMap* normalMap) {
    if (!material) {
        material = new RGBMaterial(Color(1, 1, 1));
    }
    if (!texture) {
        throw std::runtime_error("Error: Texture must be provided for a normal mapped shape.");
    }
    if (!normalMap) {
        throw std::runtime_error("Error: Normal map must be provided for a normal mapped shape.");
    }
    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
    triangle->setMaterial(material);
    triangle->setTexture(texture);
    triangle->setNormalMap(normalMap);
    return triangle;
}