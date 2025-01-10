#pragma once
#include "color.h"
#include "vec.h"
#include "ray.h"
#include "scene.h"
#include "camera.h"

class Renderer {
    // Rendering variables
    int imageWidth; // greater than 0, cant change after constructed
    int imageHeight; // greater than 0, cant changed after constructed
    int samples = 1; // greater than 0, cant change after constructed
    int maxDepth = 10; // greater than 0, cant change after constructed

public:
    // Constructor
    Renderer(int imageWidth, int imageHeight);

    // Other methods
    Color traceRay(const Ray& ray, const Scene& scene, int maxDepth);
    void render(Scene& scene, Camera& camera);
};




//// Reflection computation
//Color Rendering::computeReflection(const Ray& ray, const Vec3& point, const Vec3& N, const Vec3& I, const Scene& scene, int maxRecursion, double etaI, const Material* material) {
//    if (material->getKs() <= 0.0 || !material->isReflective()) return Color(0, 0, 0);
//
//    double cosThetaI = I.dot(N);
//    double F0 = pow((material->getEta() - etaI) / (material->getEta() + etaI), 2);
//    double Fr = F0 + (1 - F0) * pow(1 - cosThetaI, 5);
//
//    Vec3 reflectionDir = N * 2 * cosThetaI - I;
//    Ray reflectionRay(point, reflectionDir);
//    return traceRay(reflectionRay, scene, maxRecursion - 1, etaI) * Fr;
//}
//
//// Refraction computation
//Color Rendering::computeRefraction(const Ray& ray, const Vec3& point, const Vec3& N, const Vec3& I, const Scene& scene, int maxRecursion, double etaI, const Material* material, bool entering) {
//    if (material->getAlpha() >= 1.0) return Color(0, 0, 0);  // Fully opaque
//
//    double etaT = entering ? material->getEta() : 1.0;
//    double etaRatio = etaI / etaT;
//    double cosThetaI = I.dot(N);
//    double discriminant = 1 - pow(etaRatio, 2) * (1 - pow(cosThetaI, 2));
//    if (discriminant < 0) return Color(0, 0, 0);  // Total internal reflection
//
//    Vec3 TDir = (-N * sqrt(discriminant) + (N * cosThetaI - I) * etaRatio).normal();
//    Ray refractionRay(point, TDir);
//    double Fr = 1 - pow((etaT - etaI) / (etaT + etaI), 2);
//    return traceRay(refractionRay, scene, maxRecursion - 1, etaT) * (1 - Fr) * (1 - material->getAlpha());
//}