#pragma once
#include <vector>
#include "vec.h"
#include "shapes.h"
#include "ray.h"


class KDTreeNode {
    std::vector<ShapeType*> shapes; // Shapes for leaf nodes
    KDTreeNode* left;
    KDTreeNode* right;
    AABB aabb; // Bounding box

public:
    // Constructors
    KDTreeNode(const std::vector<ShapeType*>& shapes, int depth = 0);
    KDTreeNode() : left(nullptr), right(nullptr) {}

    // Destructor
    ~KDTreeNode() {
        delete left;
        delete right;
    }

    // Getters
    const AABB& getAABB() const { return aabb; }
    KDTreeNode* getLeft() const { return left; }
    KDTreeNode* getRight() const { return right; }
    const std::vector<ShapeType*>& getShapes() const { return shapes; }

    // Other methods
    KDTreeNode* findLastIntersectedNode(const Ray& ray) const;
    std::vector<KDTreeNode*> findAllIntersectedLeafNodes(const Ray& ray) const;
    void findIntersectedLeafNodesRecursive(const Ray& ray, std::vector<KDTreeNode*>& intersectedLeafNodes) const;

private:
    Vec3 computeMinBounds(const std::vector<ShapeType*>& shapes);
    Vec3 computeMaxBounds(const std::vector<ShapeType*>& shapes);
};
