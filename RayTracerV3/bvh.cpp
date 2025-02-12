#include "bvh.h"
#include <algorithm>
#include <array>
#include "shapes.h"

// AABB methods
void AABB::expand(const AABB& other) {
    minBounds = Vec3::min(this->minBounds, other.minBounds);
    maxBounds = Vec3::max(maxBounds, other.maxBounds);
}


double AABB::surfaceArea() const {
    Vec3 size = maxBounds - minBounds;
    return 2.0f * (size.x * size.y + size.y * size.z + size.z * size.x);
}


bool AABB::intersects(const Ray& ray) const {
    double tMin = (minBounds.x - ray.origin.x) / ray.getDirection().x;
    double tMax = (maxBounds.x - ray.origin.x) / ray.getDirection().x;

    if (tMin > tMax) std::swap(tMin, tMax);

    double tyMin = (minBounds.y - ray.origin.y) / ray.getDirection().y;
    double tyMax = (maxBounds.y - ray.origin.y) / ray.getDirection().y;

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax))
        return false;

    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    double tzMin = (minBounds.z - ray.origin.z) / ray.getDirection().z;
    double tzMax = (maxBounds.z - ray.origin.z) / ray.getDirection().z;

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return false;

    if (tzMin > tMin) tMin = tzMin;
    if (tzMax < tMax) tMax = tzMax;

    return tMax > 0;
}

// BVH methods
void BVHNode::buildBVH(std::vector<Shape*>& shapes, const int maxShapesPerLeaf) {
    // Compute the bounding box for all shapes
    this->aabb = AABB();
    for (auto* shape : shapes) {
            this->aabb.expand(shape->getAABB()); // Dereference the pointer to pass the AABB object
    }

    // Termination criteria: Stop if the number of shapes is below the threshold
    if (shapes.size() <= maxShapesPerLeaf) {
        this->shapes = shapes;
        return;
    }

    // Partitioning variables
    int bestAxis = -1;
    double bestCost = std::numeric_limits<double>::max();
    int bestSplit = -1;

    // Precompute sorted shapes along each axis and store them
    std::array<std::vector<Shape*>, 3> sortedShapes;
    for (int axis = 0; axis < 3; ++axis) {
        sortedShapes[axis] = shapes;
        std::sort(sortedShapes[axis].begin(), sortedShapes[axis].end(), [axis](Shape* a, Shape* b) {
            return a->getCentroid()[axis] < b->getCentroid()[axis];
            });
    }

    // Try splitting along each axis
    for (int axis = 0; axis < 3; ++axis) {
        AABB leftAABB, rightAABB;

        // Initialize rightAABB to encompass all shapes
        for (auto* shape : sortedShapes[axis]) {
            rightAABB.expand(shape->getAABB());
        }

        // Incrementally compute AABBs for left and right groups
        for (size_t i = 0; i < sortedShapes[axis].size() - 1; ++i) {
            leftAABB.expand(sortedShapes[axis][i]->getAABB());
            rightAABB = AABB(); // Recompute rightAABB from the remaining shapes
            for (size_t j = i + 1; j < sortedShapes[axis].size(); ++j) {
                rightAABB.expand(sortedShapes[axis][j]->getAABB());
            }

            // Compute SAH cost
            double cost = evaluateSAH(this->aabb, leftAABB, i + 1, rightAABB, sortedShapes[axis].size() - i - 1);
            if (cost < bestCost) {
                bestCost = cost;
                bestAxis = axis;
                bestSplit = i + 1;
            }
        }
    }

    // If no valid split was found or cost is too high, make this a leaf node
    const double terminationCostThreshold = 1e6; // Example threshold for termination
    if (bestAxis == -1 || bestCost > terminationCostThreshold) {
        this->shapes = shapes;
        return;
    }

    // Perform the best split
    std::vector<Shape*> leftShapes(sortedShapes[bestAxis].begin(), sortedShapes[bestAxis].begin() + bestSplit);
    std::vector<Shape*> rightShapes(sortedShapes[bestAxis].begin() + bestSplit, sortedShapes[bestAxis].end());

    // Recursively build child nodes
    this->left = new BVHNode();
    this->right = new BVHNode();
    this->left->buildBVH(leftShapes, maxShapesPerLeaf);
    this->right->buildBVH(rightShapes, maxShapesPerLeaf);
}


double BVHNode::evaluateSAH(const AABB& parent, const AABB& left, int leftCount, const AABB& right, int rightCount) {
    // Surface areas of the left and right bounding boxes
    double leftSurfaceArea = left.surfaceArea();
    double rightSurfaceArea = right.surfaceArea();
    double parentSurfaceArea = parent.surfaceArea();

    // Avoid division by zero (e.g., if parentSurfaceArea is degenerate)
    if (parentSurfaceArea <= 0.0f) {
        throw std::runtime_error("Invalid parent AABB with zero or negative surface area.");
    }

    // Constants for traversal and intersection costs
    const double traversalCost = 1.0; // Cost of traversing a BVH node
    const double intersectionCost = 1.0; // Cost of intersecting a shape

    // Compute SAH cost
    double sahCost = traversalCost +
        intersectionCost * ((leftSurfaceArea / parentSurfaceArea) * leftCount +
            (rightSurfaceArea / parentSurfaceArea) * rightCount);

    return sahCost;
}

// Compute minimum bounds for the given shapes
Vec3 BVHNode::computeMinBounds(const std::vector<Shape*>& shapes) {
    Vec3 minBounds = shapes[0]->getAABB().getMinBounds();
    for (auto shape : shapes) {
        Vec3 shapeMin = shape->getAABB().getMinBounds();
        minBounds = Vec3(std::min(minBounds.x, shapeMin.x), std::min(minBounds.y, shapeMin.y), std::min(minBounds.z, shapeMin.z));
    }
    return minBounds;
}

// Compute maximum bounds for the given shapes
Vec3 BVHNode::computeMaxBounds(const std::vector<Shape*>& shapes) {
    Vec3 maxBounds = shapes[0]->getAABB().getMaxBounds();
    for (auto shape : shapes) {
        Vec3 shapeMax = shape->getAABB().getMaxBounds();
        maxBounds = Vec3(std::max(maxBounds.x, shapeMax.x), std::max(maxBounds.y, shapeMax.y), std::max(maxBounds.z, shapeMax.z));
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


std::vector<BVHNode*> BVHNode::findAllIntersectedLeafNodes(const Ray& ray) const {
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


const Shape* BVHNode::findClosestIntersectedShape(const Ray& ray, Vec3& intersectionPoint) const {
    const Shape* closestShape = nullptr;
    double closestDistance = std::numeric_limits<double>::max();

    // Stack for BVH traversal
    std::vector<const BVHNode*> stack;
    stack.push_back(this);

    while (!stack.empty()) {
        const BVHNode* currentNode = stack.back();
        stack.pop_back();

        // Skip nodes that the ray does not intersect
        if (!currentNode->aabb.intersects(ray)) {
            continue;
        }

        // If this is a leaf node, check its shapes for intersections
        if (currentNode->left == nullptr && currentNode->right == nullptr) {
            for (const Shape* shape : currentNode->getShapes()) {
                Vec3 currentIntersection;
                if (shape->intersects(ray, currentIntersection)) {
                    double distance = (currentIntersection - ray.origin).length();
                    if (distance < closestDistance) {
                        closestDistance = distance;
                        closestShape = shape;
                        intersectionPoint = currentIntersection;
                    }
                }
            }
        }
        else {
            // Add child nodes to the stack for further traversal
            if (currentNode->left != nullptr) {
                stack.push_back(currentNode->left);
            }
            if (currentNode->right != nullptr) {
                stack.push_back(currentNode->right);
            }
        }
    }

    return closestShape;
}