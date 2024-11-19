#include "scene.h"

#include <limits>

const AbstractShape* Scene::findClosestIntersectedShape(const Ray& ray, Vec3& intersectionPoint) const {
    const AbstractShape* closestShape = nullptr;
    float closestDistance = std::numeric_limits<float>::max();

    // Get all intersected leaf nodes using the KDTree's method
    std::vector<BVHNode*> intersectedNodes = this->getBVHRoot()->findAllIntersectedLeafNodes(ray);

    // Iterate through all intersected nodes and check for ray-shape intersections
    for (const BVHNode* node : intersectedNodes) {
        for (const AbstractShape* shape : node->getShapes()) {  // Explicit type for shape
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
    }
    return closestShape;
}



//ColorType Ray::traceRay(const SceneType& scene) {
//    // Get all intersected leaf nodes using the KDTree's method
//    std::vector<BVHNode*> intersectedNodes = scene.getBVHRoot()->findAllIntersectedLeafNodes(*this);
//
//    // If no nodes are intersected, return the background color
//    if (intersectedNodes.empty()) {
//        return scene.getBackgroundColor();
//    }
//
//    // Iterate through all intersected nodes and check for ray-shape intersections
//    for (const auto* node : intersectedNodes) {
//        for (const auto& shape : node->getShapes()) {
//            if (shape->intersects(*this)) {
//                // Assuming shape->intersects returns true if the ray hits the shape
//                // Compute color based on shape intersection (e.g., shading, material, etc.)
//                return ColorType(1, 1, 1);  // Assuming shape has a getColor() or similar method
//            }
//        }
//    }
//
//    // If no shape was intersected, return the background color
//    return scene.getBackgroundColor();
//}
