#pragma once

#include <string>
#include "vec.h"
#include "material.h"
#include "texture.h"
#include "bump.h"
#include "ray.h"

//// Axis aligned bounding box for KDTree
class AABB {
    Vec3 max, min;

public:
    // Constructors
    AABB() : min(Vec3()), max(Vec3()) {}
    AABB(const Vec3& minBounds, const Vec3& maxBounds) : min(minBounds), max(maxBounds) {}

    // Getters
    const Vec3& getMin() const { return min; }
    const Vec3& getMax() const { return max; }

    // Setters
    void setMin(const Vec3& minBounds) { min = minBounds; }
    void setMax(const Vec3& maxBounds) { max = maxBounds; }

    // Other methods
    bool intersectsAABB(const Ray& ray) const;
};


// abstract base class for common properties
class ShapeType {
protected:
    MaterialType* material; // material has to exist cannot be nullptr

public:
    // Constructor
    ShapeType(MaterialType* material) : material(material) {}

    // Getters
    const MaterialType* getMaterial() const { return material; }

    // Setters
    void setMaterial(MaterialType* material) { this->material = material; }

    // Other methods

    // Virtual

    // Pure virtual methods
    virtual bool intersects(const Ray& ray) const = 0;
    virtual Vec3 getBoundingBoxMin() const = 0;
    virtual Vec3 getBoundingBoxMax() const = 0;

};


class SphereType : public ShapeType {
protected:
    float radius;
    Vec3 position;

public:
    // Constructor
    SphereType(Vec3 position, float radius, MaterialType* material) : position(position), radius(radius), ShapeType(material) {}

    // Getters

    // Setters

    // Other methods

    // Overides
    bool intersects(const Ray& ray) const override;
    Vec3 getBoundingBoxMin() const override {
        return Vec3(position.getX() - radius, position.getY() - radius, position.getZ() - radius);
    }

    Vec3 getBoundingBoxMax() const override {
        return Vec3(position.getX() + radius, position.getY() + radius, position.getZ() + radius);
    }
};


class TexturedSphereType : public SphereType {
protected:
    TextureType* texture;

public:
    // Constructor
    TexturedSphereType(Vec3 position, float radius, MaterialType* material, TextureType* texture) : texture(texture), SphereType(position, radius, material) {}

    // Getters

    // Setters

    // Other methods
};


class BumpTexturedSphereType : public SphereType {
protected:
    BumpType* bumpTexture;

public:
    // Constructor
    BumpTexturedSphereType(Vec3 position, float radius, MaterialType* material, BumpType* bumpTexture) : bumpTexture(bumpTexture), SphereType(position, radius, material) {}

    // Getters

    // Setters

    // Other methods
};


class TriangleType : public ShapeType {
    Vec3 *vertexA, *vertexB, *vertexC;

public:
    // Constructor
    TriangleType(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, MaterialType* material) : vertexA(vertexA), vertexB(vertexB), vertexC(vertexC), ShapeType(material) {}

    // Getters
    Vec3* getvertexA() const { return vertexA; }
    Vec3* getvertexB() const { return vertexB; }
    Vec3* getvertexC() const { return vertexC; }

    // Setters


    // Override
    bool intersects(const Ray& ray) const override;
    Vec3 getBoundingBoxMin() const override {
        return Vec3(
            std::min({ vertexA->getX(), vertexB->getX(), vertexC->getX() }),
            std::min({ vertexA->getY(), vertexB->getY(), vertexC->getY() }),
            std::min({ vertexA->getZ(), vertexB->getZ(), vertexC->getZ() })
        );
    }

    Vec3 getBoundingBoxMax() const override {
        return Vec3(
            std::max({ vertexA->getX(), vertexB->getX(), vertexC->getX() }),
            std::max({ vertexA->getY(), vertexB->getY(), vertexC->getY() }),
            std::max({ vertexA->getZ(), vertexB->getZ(), vertexC->getZ() })
        );
    }
};


class TexturedTriangleType : public TriangleType {
    TextureType* texture;
    Vec2 *textureCoord1, *textureCoord2, *textureCoord3;

public:
    // Constructor
    TexturedTriangleType(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, MaterialType* material, TextureType* texture, Vec2* textureCoord1, Vec2* textureCoord2, Vec2* textureCoord3) : textureCoord1(textureCoord1), textureCoord2(textureCoord2), textureCoord3(textureCoord3), texture(texture), TriangleType(vertexA, vertexB, vertexC, material) {}

    // Getters

    // Setters

    // Other methods
};


class SmoothShadedTriangleType : public TriangleType {
    Vec3* vertexNormal1, * vertexNormal2, *vertexNormal3;

public:
    // Constructor 
    SmoothShadedTriangleType(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, MaterialType* material, Vec3* vertexNormal1, Vec3* vertexNormal2, Vec3* vertexNormal3) : vertexNormal1(vertexNormal1), vertexNormal2(vertexNormal2), vertexNormal3(vertexNormal3), TriangleType(vertexA, vertexB, vertexC, material) {}

    // Getters

    // Setters

    // Other methods
};

class ShapeFactory