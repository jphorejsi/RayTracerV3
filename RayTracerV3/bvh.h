#pragma once
#include <vector>
#include "shapes.h"

// KD Tree for faster intersection detection
class BVHNode {
    std::vector<Shape*> shapes; // Shapes for leaf nodes
    AABB aabb; // Bounding box
    BVHNode* left; // Left branch
    BVHNode* right; // Right branch

public:
    // Constructor
    BVHNode() : left(nullptr), right(nullptr) {}

    // Destructor
    ~BVHNode() {
        delete this->left;
        delete this->right;
    }

    // Getters
    const AABB& getAABB() const { return this->aabb; }
    BVHNode* getLeft() const { return this->left; }
    BVHNode* getRight() const { return this->right; }
    const std::vector<Shape*>& getShapes() const { return this->shapes; }

    // Setters

    // Other methods
    void buildBVH(std::vector<Shape*>& shapes, const int maxShapesPerLeaf = 2);
    static double evaluateSAH(const AABB& parent, const AABB& left, int leftCount, const AABB& right, int rightCount);
    BVHNode* findLastIntersectedNode(const Ray& ray) const;
    static Vec3 computeMinBounds(const std::vector<Shape*>& shapes);
    static Vec3 computeMaxBounds(const std::vector<Shape*>& shapes);




    std::vector<BVHNode*> findAllIntersectedLeafNodes(const Ray& ray) const {
        std::vector<BVHNode*> intersectedLeafNodes;

        // Stack for iterative traversal (to avoid recursive overhead)
        std::vector<const BVHNode*> stack;
        stack.push_back(this);

        while (!stack.empty()) {
            const BVHNode* currentNode = stack.back();
            stack.pop_back();

            // Skip if the current node's AABB is not intersected by the ray
            if (!currentNode->aabb.intersects(ray)) {
                continue;
            }

            // If this is a leaf node, add it to the list
            if (currentNode->left == nullptr && currentNode->right == nullptr) {
                intersectedLeafNodes.push_back(const_cast<BVHNode*>(currentNode));
            }
            else {
                // Otherwise, add child nodes to the stack for further traversal
                if (currentNode->left != nullptr) {
                    stack.push_back(currentNode->left);
                }
                if (currentNode->right != nullptr) {
                    stack.push_back(currentNode->right);
                }
            }
        }

        return intersectedLeafNodes;
    }


    //const Shape* findClosestIntersectedShape(const Ray& ray, Vec3& intersectionPoint) const {
    //    const Shape* closestShape = nullptr;
    //    double closestDistance = std::numeric_limits<double>::max();

    //    // Stack for BVH traversal
    //    std::vector<const BVHNode*> stack;
    //    stack.push_back(this);

    //    while (!stack.empty()) {
    //        const BVHNode* currentNode = stack.back();
    //        stack.pop_back();

    //        // Skip nodes that the ray does not intersect
    //        if (!currentNode->aabb.intersects(ray)) {
    //            continue;
    //        }

    //        // If this is a leaf node, check its shapes for intersections
    //        if (currentNode->left == nullptr && currentNode->right == nullptr) {
    //            for (const Shape* shape : currentNode->getShapes()) {
    //                Vec3 currentIntersection;
    //                if (shape->intersects(ray, currentIntersection)) {
    //                    double distance = (currentIntersection - ray.origin).length();
    //                    if (distance < closestDistance) {
    //                        closestDistance = distance;
    //                        closestShape = shape;
    //                        intersectionPoint = currentIntersection;
    //                    }
    //                }
    //            }
    //        }
    //        else {
    //            // Add child nodes to the stack for further traversal
    //            if (currentNode->left != nullptr) {
    //                stack.push_back(currentNode->left);
    //            }
    //            if (currentNode->right != nullptr) {
    //                stack.push_back(currentNode->right);
    //            }
    //        }
    //    }

    //    return closestShape;
    //}
};
