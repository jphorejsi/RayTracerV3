#pragma once

#include "color.h"
#include "vec.h"
#include "scene.h"
#include "shapes.h"
#include "kdtree.h"

class Rendering {
public:
    // Ray tracing function that traverses the KDTreeNode
    Color traceRay(const Ray& ray, const Scene& scene, int depth) {
        // Step 1: Find all intersected leaf nodes in the KD tree using the root node's getter
        const KDTreeNode* kdTreeRoot = scene.getKDRoot();
        std::vector<const KDTreeNode*> intersectedNodes = kdTreeRoot->findAllIntersectedLeafNodes(ray);

        // If no nodes are intersected, return the background color
        if (intersectedNodes.empty()) {
            return scene.getBackgroundColor();
        }

        Vec3 intersectionPoint, normal;
        const IShape* closestShape = nullptr;
        float closestDistance = std::numeric_limits<float>::max();

        // Step 2: Iterate through all intersected nodes and check for ray-shape intersections using the getter method
        for (const auto* node : intersectedNodes) {
            for (const auto& shape : node->getShapes()) {
                if (shape->intersects(ray)) {
                    return Color(1, 1, 1);  // Assuming shape has a getColor() or similar method
                }
            }
        }

        // If no intersection found with any shape, return the background color
        if (!closestShape) {
            return scene.getBackgroundColor();
        }


        return color;
    }
};



