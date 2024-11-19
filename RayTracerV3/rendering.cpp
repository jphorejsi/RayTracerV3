#include "rendering.h"

#include "color.h"
#include "scene.h"
#include "shapes.h"
#include "bvh.h"
#include <vector>
#include <limits>
#include <algorithm>

// Ray tracing function that traverses the BVHNode
Color Rendering::traceRay(const Ray& ray, const Scene& scene, int depth) {
    Vec3 intersectionPoint; 

    // cull faces in the scene

    const AbstractShape* closestShape = scene.findClosestIntersectedShape(ray, intersectionPoint);

    // If no intersection found, return the background color
    if (!closestShape) {
        return scene.getBackgroundColor();
    }

    return closestShape->getMaterial()->shade(ray, intersectionPoint, scene, closestShape);
}

