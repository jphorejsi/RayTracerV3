#pragma once
#include <vector>
#include "vec.h"
#include "ray.h"

class Shape;

class AABB {
private:
    Vec3 minBounds;
    Vec3 maxBounds;

public:
    // Constructor
    AABB() : minBounds(std::numeric_limits<double>::max()), maxBounds(std::numeric_limits<double>::lowest()) {}
    AABB(const Vec3& minBounds, const Vec3& maxBounds) : minBounds(minBounds), maxBounds(maxBounds) {}

    // Getters
    Vec3 getMinBounds() const { return this->minBounds; }
    Vec3 getMaxBounds() const { return this->maxBounds; }

    // Other methods
    void expand(const AABB& other);
    double surfaceArea() const;
    bool intersects(const Ray& ray) const;

    // Operator overloads
    AABB& operator=(const AABB& other) {
        if (this != &other) {
            // Copy fields from other to this
            this->minBounds = other.minBounds;
            this->maxBounds = other.maxBounds;
        }
        return *this;
    }
};

class BVHNode {
    std::vector<Shape*> shapes; // Shapes in node
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

    // Other methods
    void buildBVH(std::vector<Shape*>& shapes, const int maxShapesPerLeaf);
    static double evaluateSAH(const AABB& parent, const AABB& left, int leftCount, const AABB& right, int rightCount);
    BVHNode* findLastIntersectedNode(const Ray& ray) const;
    static Vec3 computeMinBounds(const std::vector<Shape*>& shapes);
    static Vec3 computeMaxBounds(const std::vector<Shape*>& shapes);
    std::vector<BVHNode*> findAllIntersectedLeafNodes(const Ray& ray) const;
    const Shape* findClosestIntersectedShape(const Ray& ray, Vec3& intersectionPoint) const;
};
