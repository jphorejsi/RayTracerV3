#include "kdtree.h"
#include <algorithm> // For std::min and std::max

// Constructor to build the KD-Tree from a list of shapes
KDTreeNode::KDTreeNode(const std::vector<ShapeType*>& shapes, int depth) {
    if (shapes.empty()) {
        this->left = this->right = nullptr;
        return;
    }

    // If the max depth is reached or only one shape remains, create a leaf node
    if (depth >= 10 || shapes.size() == 1) {
        this->shapes = shapes;
        this->aabb = AABB(this->computeMinBounds(shapes), this->computeMaxBounds(shapes));
        this->left = this->right = nullptr;
        return;
    }

    // Compute the bounding box of the entire set of shapes
    this->aabb = AABB(this->computeMinBounds(shapes), this->computeMaxBounds(shapes));

    // Choose axis for splitting (0 = x, 1 = y, 2 = z)
    int axis = depth % 3;
    float midpoint;

    // Calculate midpoint based on the chosen axis
    if (axis == 0)
        midpoint = (this->aabb.getMin().getX() + this->aabb.getMax().getX()) / 2.0f;
    else if (axis == 1)
        midpoint = (this->aabb.getMin().getY() + this->aabb.getMax().getY()) / 2.0f;
    else
        midpoint = (this->aabb.getMin().getZ() + this->aabb.getMax().getZ()) / 2.0f;

    // Partition shapes into left and right children based on the midpoint
    std::vector<ShapeType*> leftShapes;
    std::vector<ShapeType*> rightShapes;

    for (auto shape : shapes) {
        Vec3 shapeMin = shape->getBoundingBoxMin();
        Vec3 shapeMax = shape->getBoundingBoxMax();
        Vec3 shapeCenter = (shapeMin + shapeMax) / 2.0f; // Center of the bounding box

        // Place the shape based on the center position relative to the midpoint
        if ((axis == 0 && shapeCenter.getX() <= midpoint) ||
            (axis == 1 && shapeCenter.getY() <= midpoint) ||
            (axis == 2 && shapeCenter.getZ() <= midpoint)) {
            leftShapes.push_back(shape);
        }
        else {
            rightShapes.push_back(shape);
        }
    }

    // If partitioning fails, create a leaf node with all shapes in this bounding box
    if (leftShapes.empty() || rightShapes.empty()) {
        this->shapes = shapes;
        this->left = this->right = nullptr;
        return;
    }

    // Recursively build the left and right subtrees
    this->left = new KDTreeNode(leftShapes, depth + 1);
    this->right = new KDTreeNode(rightShapes, depth + 1);
}

// Compute minimum bounds for the given shapes
Vec3 KDTreeNode::computeMinBounds(const std::vector<ShapeType*>& shapes) {
    Vec3 minBounds = shapes[0]->getBoundingBoxMin();
    for (auto shape : shapes) {
        Vec3 shapeMin = shape->getBoundingBoxMin();
        minBounds = Vec3(
            std::min(minBounds.getX(), shapeMin.getX()),
            std::min(minBounds.getY(), shapeMin.getY()),
            std::min(minBounds.getZ(), shapeMin.getZ())
        );
    }
    return minBounds;
}

// Compute maximum bounds for the given shapes
Vec3 KDTreeNode::computeMaxBounds(const std::vector<ShapeType*>& shapes) {
    Vec3 maxBounds = shapes[0]->getBoundingBoxMax();
    for (auto shape : shapes) {
        Vec3 shapeMax = shape->getBoundingBoxMax();
        maxBounds = Vec3(
            std::max(maxBounds.getX(), shapeMax.getX()),
            std::max(maxBounds.getY(), shapeMax.getY()),
            std::max(maxBounds.getZ(), shapeMax.getZ())
        );
    }
    return maxBounds;
}

// Find the last intersected node in the KDTree
KDTreeNode* KDTreeNode::findLastIntersectedNode(const Ray& ray) const {
    // If the ray does not intersect the current node's bounding box, return nullptr
    if (!this->aabb.intersectsAABB(ray)) {
        return nullptr;
    }

    // If this is a leaf node (no children), return this node
    if (this->left == nullptr && this->right == nullptr) {
        return const_cast<KDTreeNode*>(this); // Return non-const pointer of this node
    }

    // Initialize the results for the left and right subtrees
    KDTreeNode* leftResult = nullptr;
    KDTreeNode* rightResult = nullptr;

    // Check the left subtree
    if (this->left != nullptr) {
        leftResult = this->left->findLastIntersectedNode(ray);
    }

    // Check the right subtree
    if (this->right != nullptr) {
        rightResult = this->right->findLastIntersectedNode(ray);
    }

    // If the right subtree has an intersected node, return it
    if (rightResult != nullptr) {
        return rightResult;
    }

    // If the left subtree has an intersected node, return it
    if (leftResult != nullptr) {
        return leftResult;
    }

    // If neither subtree is intersected, return the current node (as it's intersected)
    return const_cast<KDTreeNode*>(this);
}

// Find all intersected leaf nodes
std::vector<KDTreeNode*> KDTreeNode::findAllIntersectedLeafNodes(const Ray& ray) const {
    std::vector<KDTreeNode*> intersectedLeafNodes;
    this->findIntersectedLeafNodesRecursive(ray, intersectedLeafNodes);
    return intersectedLeafNodes;
}

// Helper function for recursive search of intersected leaf nodes
void KDTreeNode::findIntersectedLeafNodesRecursive(const Ray& ray, std::vector<KDTreeNode*>& intersectedLeafNodes) const {
    // If the ray does not intersect the current node's bounding box, return
    if (!this->aabb.intersectsAABB(ray)) {
        return;
    }

    // If this is a leaf node (no children), add this node to the list
    if (this->left == nullptr && this->right == nullptr) {
        intersectedLeafNodes.push_back(const_cast<KDTreeNode*>(this));
        return;
    }

    // Recursively search for intersections in the left and right children
    if (this->left != nullptr) {
        this->left->findIntersectedLeafNodesRecursive(ray, intersectedLeafNodes);
    }
    if (this->right != nullptr) {
        this->right->findIntersectedLeafNodesRecursive(ray, intersectedLeafNodes);
    }
}