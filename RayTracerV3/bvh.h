#pragma once

#include <vector>
#include "vec.h"
#include "shapes.h"
#include "aabb.h"
#include "ray.h"


// KD Tree for faster intersection detection
class BVHNode {
    std::vector<AbstractShape*> shapes; // Shapes for leaf nodes
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
    const std::vector<AbstractShape*>& getShapes() const { return this->shapes; }

    // Setters

    // Other methods
    void buildBVH(std::vector<AbstractShape*>& shapes, const int maxShapesPerLeaf = 2);
    static double evaluateSAH(const AABB& parent, const AABB& left, int leftCount, const AABB& right, int rightCount);
    BVHNode* findLastIntersectedNode(const Ray& ray) const;
    std::vector<BVHNode*> findAllIntersectedLeafNodes(const Ray& ray) const;
    void findIntersectedLeafNodesRecursive(const Ray& ray, std::vector<BVHNode*>& intersectedLeafNodes) const;
    static Vec3 computeMinBounds(const std::vector<AbstractShape*>& shapes);
    static Vec3 computeMaxBounds(const std::vector<AbstractShape*>& shapes);
};
