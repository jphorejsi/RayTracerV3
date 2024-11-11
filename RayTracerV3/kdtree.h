#pragma once

#include <vector>
#include "vec.h"
#include "shapes.h"
#include "ray.h"

class AABB {
private:
    Vec3 minBounds;
    Vec3 maxBounds;

public:
    // Constructor
    AABB() = default;
    AABB(const Vec3& minBounds, const Vec3& maxBounds) : minBounds(minBounds), maxBounds(maxBounds) {}

    // Getters
    Vec3 getBoundingBoxMin() const { return this->minBounds; }
    Vec3 getBoundingBoxMax() const { return this->maxBounds; }

    // Setters
    void setBoundingBoxMin(const Vec3& minBounds) { this->minBounds = minBounds; }
    void setBoundingBoxMax(const Vec3& maxBounds) { this->maxBounds = maxBounds; }

    // Overrides
    bool intersects(const Ray& ray) const;
};

// KD Tree for faster intersection detection
class KDTreeNode {
    std::vector<AbstractShape*> shapes; // Shapes for leaf nodes
    AABB aabb; // Bounding box
    KDTreeNode* left; // Left branch
    KDTreeNode* right; // Right branch

public:
    // Constructors
    KDTreeNode(const std::vector<AbstractShape*>& shapes, int depth = 0);
    KDTreeNode() : left(nullptr), right(nullptr) {}

    // Destructor
    ~KDTreeNode() {
        delete this->left;
        delete this->right;
    }

    // Getters
    const AABB& getAABB() const { return this->aabb; }
    KDTreeNode* getLeft() const { return this->left; }
    KDTreeNode* getRight() const { return this->right; }
    const std::vector<AbstractShape*>& getShapes() const { return this->shapes; }

    // Setters

    // Other methods
    KDTreeNode* findLastIntersectedNode(const Ray& ray) const;
    std::vector<KDTreeNode*> findAllIntersectedLeafNodes(const Ray& ray) const;
    void findIntersectedLeafNodesRecursive(const Ray& ray, std::vector<KDTreeNode*>& intersectedLeafNodes) const;
    static Vec3 computeMinBounds(const std::vector<AbstractShape*>& shapes);
    static Vec3 computeMaxBounds(const std::vector<AbstractShape*>& shapes);
};
