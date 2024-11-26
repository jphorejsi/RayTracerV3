#include "rendering.h"

#include "color.h"
#include "scene.h"
#include "shapes.h"
#include "bvh.h"
#include <vector>
#include <limits>
#include <algorithm>


Color Rendering::traceRay(const Ray& ray, const Scene& scene, int depth) {
    // Stop recursion if the maximum depth is reached
    if (depth >= 10) {
        return scene.getBackgroundColor();
    }

    Vec3 intersectionPoint;
    const AbstractShape* closestShape = findClosestIntersectedShape(ray, scene, intersectionPoint);

    // If no intersection, return the background color
    if (!closestShape) {
        return scene.getBackgroundColor();
    }

    // Delegate shading to the shape's material
    const IMaterial* material = closestShape->getMaterial();
    if (!material) {
        return scene.getBackgroundColor();
    }

    //return material->getShadingStrategy()->shade();
}

// Finds the closest intersected shape in the scene
const AbstractShape* Rendering::findClosestIntersectedShape(const Ray& ray, const Scene& scene, Vec3& intersectionPoint) const {
    const AbstractShape* closestShape = nullptr;
    double closestDistance = std::numeric_limits<double>::infinity();

    for (const auto& shape : scene.getShapes()) {
        Vec3 tempIntersection;
        if (shape->intersect(ray, tempIntersection)) {
            double distance = (tempIntersection - ray.getOrigin()).length();
            if (distance < closestDistance) {
                closestShape = shape;
                closestDistance = distance;
                intersectionPoint = tempIntersection;
            }
        }
    }

    return closestShape;
}