#include "scene.h"

#include <limits>

const AbstractShape* Scene::findClosestIntersectedShape(const Ray& ray, Vec3& intersectionPoint) const {
    const AbstractShape* closestShape = nullptr;
    double closestDistance = std::numeric_limits<double>::max();

    // Get all intersected leaf nodes using the KDTree's method
    std::vector<BVHNode*> intersectedNodes = this->getBVHRoot()->findAllIntersectedLeafNodes(ray);

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