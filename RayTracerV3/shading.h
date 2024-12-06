#pragma once

#include "Color.h"
#include "Ray.h"
#include "Vec.h"
#include "Scene.h"
#include "Shapes.h"
#include "Lights.h"
#include "Materials.h"

// Implementation of the Phong shading strategy
class PhongShading {
public:
    static Color shadeRay(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const AbstractShape& shape);
};