#include "lights.h"

#include "scene.h"
#include <algorithm>
#include <cmath>

// DirectionalLight methods
Color DirectionalLight::calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const {
    Vec3 L = this->direction.normal() * -1.0f;  // Reverse the direction vector
    double N_dot_L = std::max(normal.dot(L), 0.0);
    return material->getOd() * material->getKd() * N_dot_L * this->color;
}

Color DirectionalLight::calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const {
    // Reverse the direction vector to point from the intersection towards the light source
    Vec3 L = this->direction.normal() * -1.0f;

    // Calculate the halfway vector
    Vec3 H = (L + viewDirection * -1.0f).normal();

    // Calculate N dot H, clamped to a minimum of 0
    double N_dot_H = std::max(normal.dot(H), 0.0);

    // Apply the Blinn-Phong specular formula
    double specularFactor = std::pow(N_dot_H, material->getN());

    // Return the calculated specular color
    return material->getOs() * material->getKs() * specularFactor * this->color;
}

// AttributeDirectionalLight methods with attenuation
Color AttributeDirectionalLight::calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const {
    Vec3 L = this->direction.normal() * -1.0f;
    double N_dot_L = std::max(normal.dot(L), 0.0);
    double dist = FLT_MAX;  // Infinite distance for directional light
    double attenuation = 1.0f / (this->c1 + this->c2 * dist + this->c3 * dist * dist);
    return material->getOd() * material->getKd() * N_dot_L * this->color * attenuation;
}

Color AttributeDirectionalLight::calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const {
    Vec3 L = this->direction.normal() * -1.0f;
    Vec3 R = (2 * normal.dot(L) * normal - L).normal();
    double R_dot_V = std::max(R.dot(viewDirection), 0.0);
    double dist = FLT_MAX;
    double attenuation = 1.0f / (this->c1 + this->c2 * dist + this->c3 * dist * dist);
    return material->getOs() * material->getKs() * std::pow(R_dot_V, material->getN()) * this->color * attenuation;
}

// PointLight methods
Color PointLight::calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const {
    Vec3 L = (this->position - intersectionPoint).normal();
    double N_dot_L = std::max(normal.dot(L), 0.0);
    return material->getOd() * material->getKd() * N_dot_L * this->color;
}

Color PointLight::calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const {
    Vec3 L = (this->position - intersectionPoint).normal();
    Vec3 R = (2 * normal.dot(L) * normal - L).normal();
    double R_dot_V = std::max(R.dot(viewDirection), 0.0);
    return material->getOs() * material->getKs() * std::pow(R_dot_V, material->getN()) * this->color;
}

// AttributePointLight methods with attenuation
Color AttributePointLight::calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const {
    Vec3 L = (this->position - intersectionPoint).normal();
    double N_dot_L = std::max(normal.dot(L), 0.0);
    double dist = (this->position - intersectionPoint).length();
    double attenuation = 1.0f / (this->c1 + this->c2 * dist + this->c3 * dist * dist);
    return material->getOd() * material->getKd() * N_dot_L * this->color * attenuation;
}

Color AttributePointLight::calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const {
    Vec3 L = (this->position - intersectionPoint).normal();
    Vec3 R = (2 * normal.dot(L) * normal - L).normal();
    double R_dot_V = std::max(R.dot(viewDirection), 0.0);
    double dist = (this->position - intersectionPoint).length();
    double attenuation = 1.0f / (this->c1 + this->c2 * dist + this->c3 * dist * dist);
    return material->getOs() * material->getKs() * std::pow(R_dot_V, material->getN()) * this->color * attenuation;
}

// Illuminates method for PointLight using BVH tree
bool PointLight::illuminates(const Vec3& position, const Scene& scene) const { // ADD INTERSECTIONPOINT
    // Create a shadow ray from the position to the light
    Vec3 L = (this->position - position).normal();
    Ray shadowRay(position, L);

    // Calculate the maximum distance to the light source
    double maxDistance = (this->position - position).length();

    // Retrieve intersected leaf nodes along the shadow ray path using the KD-tree
    std::vector<BVHNode*> intersectedNodes = scene.getBVHRoot()->findAllIntersectedLeafNodes(shadowRay);

    // Check if any shape in the intersected nodes blocks the light
    for (const BVHNode* node : intersectedNodes) {
        const std::vector<AbstractShape*>& shapes = node->getShapes();
        for (const AbstractShape* shape : shapes) {
            Vec3 currentIntersection;
            if (shape->intersects(shadowRay, currentIntersection)) {
                // Calculate distance to the intersection
                double distanceToIntersection = (currentIntersection - shadowRay.getOrigin()).length();

                // If the intersection is closer than the light source, the light is blocked
                if (distanceToIntersection < maxDistance) {
                    return false;
                }
            }
        }
    }

    // No obstruction found; the light illuminates the position
    return true;
}

// Illuminates method for DirectionalLight using KD-tree
bool DirectionalLight::illuminates(const Vec3& position, const Scene& scene) const {
    // Create a shadow ray in the opposite direction of the light source
    Vec3 directionToLight = this->direction.normal() * -1.0f;
    Ray shadowRay(position, directionToLight);

    // No maximum distance for directional lights as they are infinitely far
    std::vector<BVHNode*> intersectedNodes = scene.getBVHRoot()->findAllIntersectedLeafNodes(shadowRay);

    // Check if any shape in the intersected nodes blocks the light
    for (const BVHNode* node : intersectedNodes) {
        const std::vector<AbstractShape*>& shapes = node->getShapes();
        for (const AbstractShape* shape : shapes) {
            double t;
            Vec3 currentIntersection;
            if (shape->intersects(shadowRay, currentIntersection)) {
                // If any intersection occurs, the light is blocked
                return false;
            }
        }
    }

    // No obstruction found; the light illuminates the position
    return true;
}