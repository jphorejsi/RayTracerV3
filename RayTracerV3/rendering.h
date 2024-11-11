#pragma once

#include "color.h"
#include "scene.h"

class Rendering {
public:
    Color traceRay(const Ray& ray, const Scene& scene, int depth);
};


