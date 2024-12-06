#pragma once

#include "color.h"
#include "scene.h"
#include "shading.h"

class Rendering {
public:
    // Constructor
    Rendering(int maxDepth) {}

    // Other methods
    static Color traceRay(const Ray& ray, const Scene& scene, int maxRecursion, double etaI);
    static const AbstractShape* findClosestIntersectedShape(const Ray& ray, const Scene& scene, Vec3& intersectionPoint);
};


