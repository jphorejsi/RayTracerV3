#include "lights.h"

#include "scene.h"
#include <algorithm>
#include <cmath>

// DirectionalLight methods
Color DirectionalLight::calculateDiffuse(const BlinnPhong& material, const Vec3& intersectionPoint, const Vec3& normal, const Vec2& textureCoordinate) const {
    Vec3 L = this->direction.normal() * -1.0;  // Reverse the direction vector
    double N_dot_L = std::max(dot(normal, L), 0.0);
    return material.getOd(textureCoordinate) * material.getKd() * N_dot_L * this->color;
}

Color DirectionalLight::calculateSpecular(const BlinnPhong& material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const {
    // Reverse the direction vector to point from the intersection towards the light source
    Vec3 L = this->direction.normal() * -1.0;
    // Calculate the halfway vector
    Vec3 H = (L + -1.0 * viewDirection).normal();
    // Calculate N dot H, clamped to a minimum of 0
    double N_dot_H = std::max(dot(normal, H), 0.0);
    // Apply the Blinn-Phong specular formula
    double specularFactor = std::pow(N_dot_H, material.getN());
    // Return the calculated specular color
    return material.os * material.getKs() * specularFactor * this->color;
}

// AttributeDirectionalLight methods with attenuation
Color AttributeDirectionalLight::calculateDiffuse(const BlinnPhong& material, const Vec3& intersectionPoint, const Vec3& normal, const Vec2& textureCoordinate) const {
    Vec3 L = this->direction.normal() * -1.0;
    double N_dot_L = std::max(dot(normal, L), 0.0);
    double dist = FLT_MAX;  // Infinite distance for directional light
    double attenuation = 1.0 / (this->c1 + this->c2 * dist + this->c3 * dist * dist);
    return material.getOd(textureCoordinate) * material.getKd() * N_dot_L * this->color * attenuation;
}

Color AttributeDirectionalLight::calculateSpecular(const BlinnPhong& material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const {
    // Calculate the light direction (L), inverted to point toward the surface
    Vec3 L = (this->direction * -1.0).normal();
    // Calculate the half-vector (H)
    Vec3 H = (L + -1 * viewDirection).normal();
    // Calculate N dot H (clamp to [0, 1])
    double N_dot_H = std::max(dot(normal, H), 0.0);
    // Attenuation for directional lights is typically constant
    double attenuation = 1.0;
    // Compute the specular component
    return material.os * material.getKs() * std::pow(N_dot_H, material.getN()) * this->color * attenuation;
}


// PointLight methods
Color PointLight::calculateDiffuse(const BlinnPhong& material, const Vec3& intersectionPoint, const Vec3& normal, const Vec2& textureCoordinate) const {
    Vec3 L = (this->position - intersectionPoint).normal();
    double N_dot_L = std::max(dot(normal, L), 0.0);
    return material.getOd(textureCoordinate) * material.getKd() * N_dot_L * this->color;
}

Color PointLight::calculateSpecular(const BlinnPhong& material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const {
    // Calculate the light direction (L)
    Vec3 L = (this->position - intersectionPoint).normal();
    // Calculate the half-vector (H)
    Vec3 H = (L + -1 * viewDirection).normal(); // added -1
    // Calculate N dot H (clamp to [0, 1])
    double N_dot_H = std::max(dot(normal, H), 0.0);
    // Compute the specular component
    return material.os * material.getKs() * std::pow(N_dot_H, material.getN()) * this->color;
}


// AttributePointLight methods with attenuation
Color AttributePointLight::calculateDiffuse(const BlinnPhong& material, const Vec3& intersectionPoint, const Vec3& normal, const Vec2& textureCoordinate) const {
    Vec3 L = (this->position - intersectionPoint).normal();
    double N_dot_L = std::max(dot(normal, L), 0.0);
    double dist = (this->position - intersectionPoint).length();
    double attenuation = 1.0 / (this->c1 + this->c2 * dist + this->c3 * dist * dist);
    return material.getOd(textureCoordinate) * material.getKd() * N_dot_L * this->color * attenuation;
}

Color AttributePointLight::calculateSpecular(const BlinnPhong& material, const Vec3& intersectionPoint, const Vec3& normal, const Vec3& viewDirection) const {
    // Calculate the light direction (L)
    Vec3 L = (this->position - intersectionPoint).normal();
    // Calculate the half-vector (H)
    Vec3 H = (L + -1 * viewDirection).normal(); // Added -1 to invert view direction
    // Calculate N dot H (clamp to [0, 1])
    double N_dot_H = std::max(dot(normal, H), 0.0);
    // Calculate the distance between light and intersection
    double dist = (this->position - intersectionPoint).length();
    // Compute attenuation factor
    double attenuation = 1.0 / (this->c1 + this->c2 * dist + this->c3 * dist * dist);
    // Compute the specular component
    return material.os * material.getKs() * std::pow(N_dot_H, material.getN()) * this->color * attenuation;
}


double DirectionalLight::calculateShadowFactor(const Vec3& intersectionPoint, const Scene& scene) const {
    Vec3 shadowDirection = -1 * direction.normal();
    Ray shadowRay(intersectionPoint, shadowDirection);
    double shadowFactor = 1.0;

    // Get all intersected leaf nodes
    std::vector<BVHNode*> intersectedNodes = scene.getBVHRoot()->findAllIntersectedLeafNodes(shadowRay);
    for (const BVHNode* node : intersectedNodes) {
        for (const Shape* shape : node->getShapes()) {
            Vec3 tempIntersection;
            if (shape->intersects(shadowRay, tempIntersection)) {
                // Check if intersection point is valid (avoid self-shadowing)
                double epsilon = 1e-5; // Small offset to prevent self-intersection
                double distanceToIntersection = (tempIntersection - intersectionPoint).length();
                if (distanceToIntersection > epsilon) {
                    const Material& material = shape->getMaterial();
                    shadowFactor *= (1.0 - material.getAlpha());
                    if (shadowFactor <= 0) {
                        return 0; // Fully shadowed
                    }
                }
            }
        }
    }
    return shadowFactor;
}

double PointLight::calculateShadowFactor(const Vec3& intersectionPoint, const Scene& scene) const {
    Vec3 shadowDirection = (position - intersectionPoint).normal();
    Ray shadowRay(intersectionPoint, shadowDirection);
    double shadowFactor = 1.0;

    // Get all intersected leaf nodes
    std::vector<BVHNode*> intersectedNodes = scene.getBVHRoot()->findAllIntersectedLeafNodes(shadowRay);
    for (const BVHNode* node : intersectedNodes) {
        for (const Shape* shape : node->getShapes()) {
            Vec3 tempIntersection;
            if (shape->intersects(shadowRay, tempIntersection)) {
                // Calculate distances to ensure valid occlusion
                double distanceToLight = (position - intersectionPoint).length();
                double distanceToIntersection = (tempIntersection - intersectionPoint).length();
                double epsilon = 1e-5; // Small offset to prevent self-intersection

                if (distanceToIntersection > epsilon && distanceToIntersection < distanceToLight) {
                    const Material& material = shape->getMaterial();
                    shadowFactor *= (1.0 - material.getAlpha());
                    if (shadowFactor <= 0) {
                        return 0; // Fully shadowed
                    }
                }
            }
        }
    }
    return shadowFactor;
}