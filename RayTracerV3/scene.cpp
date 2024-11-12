#include "scene.h"

#include <limits>

const AbstractShape* Scene::findClosestIntersectedShape(const Ray& ray, Vec3& intersectionPoint) const {
    const AbstractShape* closestShape = nullptr;
    float closestDistance = std::numeric_limits<float>::max();

    for (const AbstractShape* shape : shapes) {
        Vec3 currentIntersection;
        if (shape->intersects(ray, currentIntersection)) {
            float distance = (currentIntersection - ray.getOrigin()).length();
            if (distance < closestDistance) {
                closestDistance = distance;
                closestShape = shape;
                intersectionPoint = currentIntersection;
            }
        }
    }
    return closestShape;
}