#include "rendering.h"

#include "color.h"
#include "scene.h"
#include "shapes.h"
#include "bvh.h"
#include <vector>
#include <limits>
#include <algorithm>
#include "Shading.h"
#include <cmath> // For pow, sqrt

//Color Rendering::traceRay(const Ray& ray, const Scene& scene, int maxRecursion, double etaI) {
//    // Stop recursion if the maximum depth is reached
//    if (maxRecursion < 0) {
//        return Color(0, 0, 0); // Black
//    }
//
//    Vec3 intersectionPoint;
//    const AbstractShape* closestShape = findClosestIntersectedShape(ray, scene, intersectionPoint);
//
//    // If no intersection, return the background color
//    if (!closestShape) {
//        return scene.getBackgroundColor();
//    }
//
//    // Get material and normal
//    Material* material = closestShape->getMaterial();
//    if (!material) {
//        return Color(0.5, 0.5, 0.5); // Default gray
//    }
//
//    Vec3 N = closestShape->getNormal(intersectionPoint).normal();
//    Vec3 I = ray.getDirection().normal() * -1;
//
//    // Check if the ray is entering or exiting the object
//    bool entering = I.dot(N) > 0;
//    if (!entering) {
//        N = N * -1; // Flip normal for exiting rays
//    }
//
//    // Compute illumination at the intersection point
//    Color illumination = PhongShading::shadeRay(ray, intersectionPoint, scene, *closestShape);
//
//    // Material properties
//    double alpha = material->getAlpha(); // Opacity
//    double etaT = entering ? material->getEta() : 1.0; // Refraction index
//    double etaRatio = etaI / etaT;
//
//    // Compute Fresnel reflectance using Schlick's approximation
//    double cosThetaI = I.dot(N);
//    double F0 = pow((etaT - etaI) / (etaT + etaI), 2);
//    double Fr = F0 + (1 - F0) * pow(1 - cosThetaI, 5);
//
//    // Compute reflection
//    Color reflection(0, 0, 0);
//    if (material->getKs() > 0.0) { // Reflect if material has specular component
//        Vec3 reflectionDirection = N * 2 * cosThetaI - I;
//        Ray reflectionRay(intersectionPoint, reflectionDirection);
//        reflection = traceRay(reflectionRay, scene, maxRecursion - 1, etaI) * Fr;
//    }
//
//    // Compute refraction
//    Color refraction(0, 0, 0);
//    if (alpha < 1.0) { // Only calculate refraction if material is not fully opaque
//        double discriminant = 1 - pow(etaRatio, 2) * (1 - pow(cosThetaI, 2));
//        if (discriminant >= 0) { // Total internal reflection check
//            //Vec3 TDir = ((I - N * cosThetaI) * etaRatio - N * sqrt(discriminant)).normal();
//            Vec3 TDir = (((N * -1) * sqrt(discriminant)) + (((N * alpha) - I)) * (etaI / etaT)).normal();
//            Ray refractionRay(intersectionPoint, TDir);
//            refraction = traceRay(refractionRay, scene, maxRecursion - 1, etaT) * (1 - Fr) * (1 - alpha);
//        }
//    }
//
//    // Combine illumination, reflection, and refraction
//    //Color finalColor = illumination * alpha + reflection + refraction;
//    Color finalColor = illumination + reflection + refraction;
//
//    // Manually constrain the final color to [0.0, 1.0]
//    finalColor.setR(finalColor.getR() < 0.0 ? 0.0 : (finalColor.getR() > 1.0 ? 1.0 : finalColor.getR()));
//    finalColor.setG(finalColor.getG() < 0.0 ? 0.0 : (finalColor.getG() > 1.0 ? 1.0 : finalColor.getG()));
//    finalColor.setB(finalColor.getB() < 0.0 ? 0.0 : (finalColor.getB() > 1.0 ? 1.0 : finalColor.getB()));
//
//    return finalColor;
//}


//Color Rendering::traceRay(const Ray& ray, const Scene& scene, int maxRecursion, double etaI) {
//    // Stop recursion if the maximum depth is reached
//    if (maxRecursion < 0) {
//        return Color(0, 0, 0); // Black
//    }
//
//    Vec3 intersectionPoint;
//    const AbstractShape* closestShape = findClosestIntersectedShape(ray, scene, intersectionPoint);
//
//    // If no intersection, return the background color
//    if (!closestShape) {
//        return scene.getBackgroundColor();
//    }
//
//    // Get material and normal
//    Material* material = closestShape->getMaterial();
//    if (!material) {
//        return Color(0.5, 0.5, 0.5); // Default gray
//    }
//
//    Vec3 N = closestShape->getNormal(intersectionPoint).normal();
//    Vec3 I = ray.getDirection().normal() * -1;
//
//    // Compute illumination at the intersection point
//    Color illumination = PhongShading::shadeRay(ray, intersectionPoint, scene, *closestShape);
//
//    // Calculate Fresnel reflectance
//    double alpha = N.dot(I);
//    if (alpha < 0) {
//        N = N * -1;
//        alpha = N.dot(I);
//    }
//
//    double etaT = material->getEta();
//    double F0 = pow((etaT - etaI) / (etaT + etaI), 2);
//    double Fr = F0 + (1 - F0) * pow(1 - alpha, 5);
//
//    // Compute reflection
//    Color reflection(0, 0, 0);
//    if (material->getAlpha() > 0.0) { // Reflect if alpha is greater than 0
//        Vec3 reflectionDirection = N * 2 * alpha - I;
//        Ray reflectionRay(intersectionPoint, reflectionDirection);
//        Color ref = traceRay(reflectionRay, scene, maxRecursion - 1, etaI); // fix
//        reflection = ref * Fr;
//    }
//
//    // Reset etaT to 1 if exiting material
//    if (etaI == etaT) {
//        etaT = 1;
//    }
//
//    // Compute refraction
//    Color refraction(0, 0, 0);
//    if (material->getAlpha() < 1.0 && material->getEta() > 1.0) {
//        double discriminant = 1 - pow(etaI / etaT, 2) * (1 - pow(alpha, 2));
//        if (discriminant >= 0) {
//            Vec3 TDir = ((-1 * N * sqrt(discriminant)) + ((N * alpha) - I) * (etaI / etaT)).normal();
//            Ray refractionRay(intersectionPoint, TDir);
//            refraction = traceRay(refractionRay, scene, maxRecursion - 1, etaT) * (1 - Fr) * (1 - material->getAlpha());
//        }
//    }
//
//    // Combine illumination, reflection, and refraction
//    Color finalColor = illumination + reflection + refraction;
//
//    // Clamp the final color
//    if (finalColor.getR() < 0.0f) {
//        finalColor.setR(0.0f);
//    }
//    else if (finalColor.getR() > 1.0f) {
//        finalColor.setR(1.0f);
//    }
//
//    if (finalColor.getG() < 0.0f) {
//        finalColor.setG(0.0f);
//    }
//    else if (finalColor.getG() > 1.0f) {
//        finalColor.setG(1.0f);
//    }
//
//    if (finalColor.getB() < 0.0f) {
//        finalColor.setB(0.0f);
//    }
//    else if (finalColor.getB() > 1.0f) {
//        finalColor.setB(1.0f);
//    }
//
//    return finalColor;
//}

Color Rendering::traceRay(const Ray& ray, const Scene& scene, int maxRecursion, double etaI) {
    // Stop recursion if the maximum depth is reached
    if (maxRecursion < 0) {
        return Color(0, 0, 0); // Black
    }

    Vec3 intersectionPoint;
    const AbstractShape* closestShape = findClosestIntersectedShape(ray, scene, intersectionPoint);

    // If no intersection, return the background color
    if (!closestShape) {
        return scene.getBackgroundColor();
    }

    // Get material and normal
    Material* material = closestShape->getMaterial();
    if (!material) {
        return Color(0.5, 0.5, 0.5); // Default gray
    }

    Vec3 N = closestShape->getNormal(intersectionPoint).normal();
    Vec3 I = ray.getDirection().normal() * -1;

    // Check if the ray is entering or exiting the object
    bool entering = I.dot(N) > 0;
    if (!entering) {
        N = N * -1; // Flip normal for exiting rays
    }

    // Compute illumination at the intersection point
    Color illumination = PhongShading::shadeRay(ray, intersectionPoint, scene, *closestShape);

    // Material properties
    double etaT = entering ? material->getEta() : 1.0; // Refraction index
    double etaRatio = etaI / etaT;

    // Compute Fresnel reflectance using Schlick's approximation
    double cosThetaI = I.dot(N);
    double F0 = pow((etaT - etaI) / (etaT + etaI), 2);
    double Fr = F0 + (1 - F0) * pow(1 - cosThetaI, 5);

    // Compute reflection
    Color reflection(0, 0, 0);
    if (material->getKs() > 0.0) { // Reflect if material has specular component
        Vec3 reflectionDirection = N * 2 * cosThetaI - I;
        Ray reflectionRay(intersectionPoint, reflectionDirection); // No offset
        reflection = traceRay(reflectionRay, scene, maxRecursion - 1, etaI) * Fr;
    }

    // Compute refraction
    Color refraction(0, 0, 0);
    if (material->getEta() > 1.0) { // Only calculate refraction if material has refraction properties
        double discriminant = 1 - pow(etaRatio, 2) * (1 - pow(cosThetaI, 2));
        if (discriminant >= 0) { // Total internal reflection check
            Vec3 TDir = ((-1 * N * sqrt(discriminant)) + ((N * cosThetaI) - I) * etaRatio).normal();
            Ray refractionRay(intersectionPoint, TDir); // No offset
            refraction = traceRay(refractionRay, scene, maxRecursion - 1, etaT) * (1 - Fr) * (1 - material->getAlpha());
        }
    }

    // Combine illumination, reflection, and refraction
    Color finalColor = illumination + reflection + refraction;

    // Manually constrain the final color to [0.0, 1.0]
    finalColor.setR(finalColor.getR() < 0.0 ? 0.0 : (finalColor.getR() > 1.0 ? 1.0 : finalColor.getR()));
    finalColor.setG(finalColor.getG() < 0.0 ? 0.0 : (finalColor.getG() > 1.0 ? 1.0 : finalColor.getG()));
    finalColor.setB(finalColor.getB() < 0.0 ? 0.0 : (finalColor.getB() > 1.0 ? 1.0 : finalColor.getB()));

    return finalColor;
}


const AbstractShape* Rendering::findClosestIntersectedShape(const Ray& ray, const Scene& scene, Vec3& intersectionPoint) {
    const AbstractShape* closestShape = nullptr;
    double closestDistance = std::numeric_limits<double>::max();

    // Get all intersected leaf nodes using the KDTree's method
    std::vector<BVHNode*> intersectedNodes = scene.getBVHRoot()->findAllIntersectedLeafNodes(ray);
    
    // Iterate through all intersected nodes and check for ray-shape intersections
    for (const BVHNode* node : intersectedNodes) {
        for (const AbstractShape* shape : node->getShapes()) {  // Explicit type for shape
            Vec3 currentIntersection;
            if (shape->intersects(ray, currentIntersection)) {
                double distance = (currentIntersection - ray.getOrigin()).length();
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestShape = shape;
                    intersectionPoint = currentIntersection;
                }
            }
        }
    }
    return closestShape;
}