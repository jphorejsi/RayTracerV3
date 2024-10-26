#include "ray.h"

//ColorType Ray::traceRay(const SceneType& scene) {
//    // Get all intersected leaf nodes using the KDTree's method
//    std::vector<KDTreeNode*> intersectedNodes = scene.getKDRoot()->findAllIntersectedLeafNodes(*this);
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
