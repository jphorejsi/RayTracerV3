#pragma once

#include <memory> // For std::shared_ptr
#include "vec.h"
#include "materials.h"
#include "texture.h"
#include "ray.h"
#include "color.h"
#include "aabb.h"
#include <vector>
#include <fstream>
#include <sstream>

// Abstract base class for shapes
class Shape {
protected:
    AABB aabb;
    std::shared_ptr<Material> material;

public:
    // Constructor
    Shape(const std::shared_ptr<Material>& matPtr) : aabb(), material(matPtr) {}
    virtual ~Shape() = default;

    // Getters
    const AABB& getAABB() const { return aabb; }

    // Return a const reference to the Material by dereferencing material
    const Material& getMaterial() const {
        return *material;
    }

    // Pure virtual methods
    virtual bool intersects(const Ray& ray, Vec3& intersectionPoint) const = 0;
    virtual Vec2 getTextureCoordinate(const Vec3& intersectionPoint) const = 0;
    virtual Vec3 getNormal(const Vec3& intersectionPoint) const = 0;
    virtual Vec3 getCentroid() const = 0;
};

// Concrete Sphere class derived from Shape
class Sphere : public Shape {
private:
    double radius;
    
public:
    Vec3 position;

    // Constructor
    Sphere(const Vec3& position, const double radius, const std::shared_ptr<Material>& material) : Shape(material), position(position), radius(radius) {
        if (radius < 0) {
            throw std::runtime_error("Error: Degenerate Sphere with zero area.");
        }
        Vec3 radiusVec(radius, radius, radius);
        this->aabb = AABB(position - radiusVec, position + radiusVec);
    }

    // Setters
    void setRadius(const double radius) { this->radius = radius; } // update
    void setPosition(const Vec3 position) { this->position = position; } // update

    // Override methods
    bool intersects(const Ray& ray, Vec3& intersectionPoint) const override;
    Vec2 getTextureCoordinate(const Vec3& intersectionPoint) const override;
    Vec3 getNormal(const Vec3& intersectionPoint) const override;
    Vec3 getCentroid() const override { return position; }
};

// Concrete Triangle class derived from Shape
class Triangle : public Shape {
private:
    Vec3* vertexANormal = nullptr;
    Vec3* vertexBNormal = nullptr;
    Vec3* vertexCNormal = nullptr;
    Vec2* textureCoordinateA = nullptr;
    Vec2* textureCoordinateB = nullptr;
    Vec2* textureCoordinateC = nullptr;

public:
    Vec3& vertexA;
    Vec3& vertexB;
    Vec3& vertexC;

    // Constructor
    Triangle(Vec3& vertexA, Vec3& vertexB, Vec3& vertexC, const std::shared_ptr<Material>& material) : Shape(material), vertexA(vertexA), vertexB(vertexB), vertexC(vertexC) {
        // Calculate the minimum and maximum bounds
        Vec3 minBounds(
            std::min({ vertexA.x, vertexB.x, vertexC.x }),
            std::min({ vertexA.y, vertexB.y, vertexC.y }),
            std::min({ vertexA.z, vertexB.z, vertexC.z })
        );

        Vec3 maxBounds(
            std::max({ vertexA.x, vertexB.x, vertexC.x }),
            std::max({ vertexA.y, vertexB.y, vertexC.y }),
            std::max({ vertexA.z, vertexB.z, vertexC.z })
        );

        // Set the bounds of the AABB
        this->aabb = AABB(minBounds, maxBounds);
    }

    // Setters for normals and texture coordinates
    void setVertexANormal(Vec3* normal) { vertexANormal = normal; }
    void setVertexBNormal(Vec3* normal) { vertexBNormal = normal; }
    void setVertexCNormal(Vec3* normal) { vertexCNormal = normal; }
    void setTextureCoordinateA(Vec2* coord) { textureCoordinateA = coord; }
    void setTextureCoordinateB(Vec2* coord) { textureCoordinateB = coord; }
    void setTextureCoordinateC(Vec2* coord) { textureCoordinateC = coord; }

    // Override methods
    bool intersects(const Ray& ray, Vec3& intersectionPoint) const override;
    Vec2 getTextureCoordinate(const Vec3& intersectionPoint) const override;
    Vec3 getNormal(const Vec3& intersectionPoint) const override;
    Vec3 getCentroid() const override {
        return (vertexA + vertexB + vertexC) / 3.0f;
    }

    //other methods
    Vec3 getBarycentricCoordinates(const Vec3& intersectionPoint) const;
};


//class Cylinder : public Shape {
//
//
//};