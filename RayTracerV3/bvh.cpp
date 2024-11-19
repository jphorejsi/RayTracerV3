#include "bvh.h"
#include <algorithm>


// Determine if ray intersects AABB
bool AABB::intersects(const Ray& ray) const {
    float invDirX = 1.0f / ray.getDirection().getX();
    float invDirY = 1.0f / ray.getDirection().getY();
    float invDirZ = 1.0f / ray.getDirection().getZ();

    float tMin = (this->minBounds.getX() - ray.getOrigin().getX()) * invDirX;
    float tMax = (this->maxBounds.getX() - ray.getOrigin().getX()) * invDirX;

    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (this->minBounds.getY() - ray.getOrigin().getY()) * invDirY;
    float tyMax = (this->maxBounds.getY() - ray.getOrigin().getY()) * invDirY;

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax)) return false;

    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    float tzMin = (this->minBounds.getZ() - ray.getOrigin().getZ()) * invDirZ;
    float tzMax = (this->maxBounds.getZ() - ray.getOrigin().getZ()) * invDirZ;

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax)) return false;

    return true;
}

void BVHNode::buildBVH(std::vector<AbstractShape*>& shapes, const int maxShapesPerLeaf) {
    // Compute the bounding box for all shapes
    this->aabb = AABB();
    for (auto* shape : shapes) {
        if (const AABB* shapeAABB = shape->getAABB()) { // Check if AABB is valid
            this->aabb.expand(*shapeAABB); // Dereference the pointer to pass the AABB object
        }
    }

    // Termination criteria: Stop if the number of shapes is below the threshold
    if (shapes.size() <= maxShapesPerLeaf) {
        this->shapes = shapes;
        return;
    }

    // Partitioning variables
    int bestAxis = -1;
    float bestCost = std::numeric_limits<float>::max();
    int bestSplit = -1;

    // Try splitting along each axis
    for (int axis = 0; axis < 3; ++axis) {
        // Sort shapes by centroid along the current axis
        std::sort(shapes.begin(), shapes.end(), [axis](AbstractShape* a, AbstractShape* b) {
            return a->getCentroid()[axis] < b->getCentroid()[axis];
            });

        // Evaluate split positions
        for (size_t i = 1; i < shapes.size(); ++i) {
            // Split into two groups
            std::vector<AbstractShape*> leftShapes(shapes.begin(), shapes.begin() + i);
            std::vector<AbstractShape*> rightShapes(shapes.begin() + i, shapes.end());

            // Compute AABBs for the two groups
            AABB leftAABB, rightAABB;
            for (auto* shape : leftShapes) leftAABB.expand(*shape->getAABB());
            for (auto* shape : rightShapes) rightAABB.expand(*shape->getAABB());

            // Compute SAH cost
            float cost = evaluateSAH(this->aabb, leftAABB, leftShapes.size(), rightAABB, rightShapes.size());
            if (cost < bestCost) {
                bestCost = cost;
                bestAxis = axis;
                bestSplit = i;
            }
        }
    }

    // If no valid split was found, make this a leaf node
    if (bestAxis == -1) {
        this->shapes = shapes;
        return;
    }

    // Perform the best split
    std::sort(shapes.begin(), shapes.end(), [bestAxis](AbstractShape* a, AbstractShape* b) {
        return a->getCentroid()[bestAxis] < b->getCentroid()[bestAxis];
        });

    std::vector<AbstractShape*> leftShapes(shapes.begin(), shapes.begin() + bestSplit);
    std::vector<AbstractShape*> rightShapes(shapes.begin() + bestSplit, shapes.end());

    // Recursively build child nodes
    this->left = new BVHNode();
    this->right = new BVHNode();
    this->left->buildBVH(leftShapes, maxShapesPerLeaf);
    this->right->buildBVH(rightShapes, maxShapesPerLeaf);
}

float BVHNode::evaluateSAH(const AABB& parent, const AABB& left, int leftCount, const AABB& right, int rightCount) {
    // Surface areas of the left and right bounding boxes
    float leftSurfaceArea = left.surfaceArea();
    float rightSurfaceArea = right.surfaceArea();
    float parentSurfaceArea = parent.surfaceArea();

    // Avoid division by zero (e.g., if parentSurfaceArea is degenerate)
    if (parentSurfaceArea <= 0.0f) {
        throw std::runtime_error("Invalid parent AABB with zero or negative surface area.");
    }

    // Constants for traversal and intersection costs
    const float traversalCost = 1.0f; // Cost of traversing a BVH node
    const float intersectionCost = 1.0f; // Cost of intersecting a shape

    // Compute SAH cost
    float sahCost = traversalCost +
        intersectionCost * ((leftSurfaceArea / parentSurfaceArea) * leftCount +
            (rightSurfaceArea / parentSurfaceArea) * rightCount);

    return sahCost;
}

// Compute minimum bounds for the given shapes
Vec3 BVHNode::computeMinBounds(const std::vector<AbstractShape*>& shapes) {
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
Vec3 BVHNode::computeMaxBounds(const std::vector<AbstractShape*>& shapes) {
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
BVHNode* BVHNode::findLastIntersectedNode(const Ray& ray) const {
    // If the ray does not intersect the current node's bounding box, return nullptr
    if (!this->aabb.intersects(ray)) {
        return nullptr;
    }

    // If this is a leaf node (no children), return this node
    if (this->left == nullptr && this->right == nullptr) {
        return const_cast<BVHNode*>(this); // Return non-const pointer of this node
    }

    // Initialize the results for the left and right subtrees
    BVHNode* leftResult = nullptr;
    BVHNode* rightResult = nullptr;

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
    return const_cast<BVHNode*>(this);
}

// Find all intersected leaf nodes
std::vector<BVHNode*> BVHNode::findAllIntersectedLeafNodes(const Ray& ray) const {
    std::vector<BVHNode*> intersectedLeafNodes;
    this->findIntersectedLeafNodesRecursive(ray, intersectedLeafNodes);
    return intersectedLeafNodes;
}

// Helper function for recursive search of intersected leaf nodes
void BVHNode::findIntersectedLeafNodesRecursive(const Ray& ray, std::vector<BVHNode*>& intersectedLeafNodes) const {
    // If the ray does not intersect the current node's bounding box, return
    if (!this->aabb.intersects(ray)) {
        return;
    }

    // If this is a leaf node (no children), add this node to the list
    if (this->left == nullptr && this->right == nullptr) {
        intersectedLeafNodes.push_back(const_cast<BVHNode*>(this));
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
