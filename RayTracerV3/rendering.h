#pragma once

#include "color.h"
#include "scene.h"

class Rendering {
public:
    Color traceRay(const Ray& ray, const Scene& scene, int depth);
    Color shadeRay(const Ray& ray, const Scene& scene, const IShape* shape, const Vec3& IntersectionPoint);
};


