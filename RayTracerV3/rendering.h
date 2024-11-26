#pragma once

#include "color.h"
#include "scene.h"

class Rendering {
private:
    int maxDepth = 10;

public:
    // Constructor
    Rendering(int maxDepth) : maxDepth(maxDepth) {}

    // Other methods
    Color traceRay(const Ray& ray, const Scene& scene, int depth);
    const AbstractShape* findClosestIntersectedShape(const Ray& ray, const Scene& scene, Vec3& intersectionPoint) const;
};


