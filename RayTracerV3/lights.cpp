#include "lights.h"
#include <algorithm>
#include <cmath>

// DirectionalLight methods
Color DirectionalLight::calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const {
    Vec3 L = this->direction.normal() * -1.0f;  // Reverse the direction vector
    float N_dot_L = std::max(normal.dot(L), 0.0f);
    return material->getOd() * material->getKd() * N_dot_L * this->color;
}

Color DirectionalLight::calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const {
    Vec3 L = this->direction.normal() * -1.0f;  // Reverse the direction vector
    Vec3 R = (2 * normal.dot(L) * normal - L).normal();
    float R_dot_V = std::max(R.dot(viewDirection), 0.0f);
    return material->getOs() * material->getKs() * std::pow(R_dot_V, material->getN()) * this->color;
}

// AttributeDirectionalLight methods with attenuation
Color AttributeDirectionalLight::calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const {
    Vec3 L = this->direction.normal() * -1.0f;
    float N_dot_L = std::max(normal.dot(L), 0.0f);
    float dist = FLT_MAX;  // Infinite distance for directional light
    float attenuation = 1.0f / (this->c1 + this->c2 * dist + this->c3 * dist * dist);
    return material->getOd() * material->getKd() * N_dot_L * this->color * attenuation;
}

Color AttributeDirectionalLight::calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const {
    Vec3 L = this->direction.normal() * -1.0f;
    Vec3 R = (2 * normal.dot(L) * normal - L).normal();
    float R_dot_V = std::max(R.dot(viewDirection), 0.0f);
    float dist = FLT_MAX;
    float attenuation = 1.0f / (this->c1 + this->c2 * dist + this->c3 * dist * dist);
    return material->getOs() * material->getKs() * std::pow(R_dot_V, material->getN()) * this->color * attenuation;
}

// PointLight methods
Color PointLight::calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const {
    Vec3 L = (this->position - intersectionPoint).normal();
    float N_dot_L = std::max(normal.dot(L), 0.0f);
    return material->getOd() * material->getKd() * N_dot_L * this->color;
}

Color PointLight::calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const {
    Vec3 L = (this->position - intersectionPoint).normal();
    Vec3 R = (2 * normal.dot(L) * normal - L).normal();
    float R_dot_V = std::max(R.dot(viewDirection), 0.0f);
    return material->getOs() * material->getKs() * std::pow(R_dot_V, material->getN()) * this->color;
}

// AttributePointLight methods with attenuation
Color AttributePointLight::calculateDiffuse(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal) const {
    Vec3 L = (this->position - intersectionPoint).normal();
    float N_dot_L = std::max(normal.dot(L), 0.0f);
    float dist = (this->position - intersectionPoint).length();
    float attenuation = 1.0f / (this->c1 + this->c2 * dist + this->c3 * dist * dist);
    return material->getOd() * material->getKd() * N_dot_L * this->color * attenuation;
}

Color AttributePointLight::calculateSpecular(const PhongMaterial* material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const {
    Vec3 L = (this->position - intersectionPoint).normal();
    Vec3 R = (2 * normal.dot(L) * normal - L).normal();
    float R_dot_V = std::max(R.dot(viewDirection), 0.0f);
    float dist = (this->position - intersectionPoint).length();
    float attenuation = 1.0f / (this->c1 + this->c2 * dist + this->c3 * dist * dist);
    return material->getOs() * material->getKs() * std::pow(R_dot_V, material->getN()) * this->color * attenuation;
}

bool PointLight::illuminates(const Vec3& position, const Scene& scene) const {
    // Create a shadow ray from the position towards the light source
    Vec3 directionToLight = (this->position - position).normal();
    Ray shadowRay(position, directionToLight);

    // Calculate distance to the light source
    float maxDistance = (this->position - position).length();

    // Traverse KDTree to find if any object intersects with this shadow ray
    KDTreeNode* intersectedNode = scene.getKDTree().findLastIntersectedNode(shadowRay);

    if (!intersectedNode) return true;  // No intersection means the light illuminates the point

    // Check if any shape in the intersected leaf node blocks the light
    for (const auto& shape : intersectedNode->getShapes()) {
        float t;
        if (shape->intersects(shadowRay, t) && t < maxDistance) {
            return false;  // Intersection with an object before reaching the light source
        }
    }

    return true;  // No obstruction found, light illuminates the position
}

// Illuminates method for DirectionalLight
bool DirectionalLight::illuminates(const Vec3& position) const {
    // Create a shadow ray in the opposite direction of the light
    Vec3 directionToLight = this->direction.normal() * -1.0f;
    Ray shadowRay(position, directionToLight);

    // Since directional lights are infinitely far away, we don’t set a max distance
    KDTreeNode* intersectedNode = scene.getKDTree().findLastIntersectedNode(shadowRay);

    if (!intersectedNode) return true;  // No intersection means the light illuminates the point

    // Check if any shape in the intersected leaf node blocks the light
    for (const auto& shape : intersectedNode->getShapes()) {
        float t;
        if (shape->intersects(shadowRay, t)) {
            return false;  // Intersection with an object, blocking the directional light
        }
    }

    return true;  // No obstruction found, light illuminates the position
}