#pragma once

#include "vec.h"
#include "materials.h"
#include "texture.h"
#include "ray.h"
#include "color.h"

// Abstract base class for shapes
class AbstractShape {
protected:
    IMaterial* material = nullptr;
    Texture* texture = nullptr;
    NormalMap* normalMap = nullptr;

public:
    virtual ~AbstractShape() = default;

    // Getters for shared properties
    IMaterial* getMaterial() const { return material; }
    Texture* getTexture() const { return texture; }
    NormalMap* getNormalMap() const { return normalMap; }

    // Setters for shared properties
    void setMaterial(IMaterial* mat) { material = mat; }
    void setTexture(Texture* tex) { texture = tex; }
    void setNormalMap(NormalMap* nMap) { normalMap = nMap; }

    // Pure virtual methods for shape-specific operations
    virtual bool intersects(const Ray& ray, Vec3& intersectionPoint) const = 0;
    virtual Vec3 getBoundingBoxMin() const = 0;
    virtual Vec3 getBoundingBoxMax() const = 0;
    virtual Vec2 calculateTextureCoordinate(const Vec3& intersectionPoint) const = 0;
};

// Concrete Sphere class derived from AbstractShape
class Sphere : public AbstractShape {
private:
    float radius;
    Vec3 position;

public:
    // Constructor
    Sphere(const Vec3& position, float radius) : position(position), radius(radius) {}

    // Override methods
    bool intersects(const Ray& ray, Vec3& intersectionPoint) const override;
    Vec3 getBoundingBoxMin() const override;
    Vec3 getBoundingBoxMax() const override;
    Vec2 calculateTextureCoordinate(const Vec3& intersectionPoint) const override;
};

// Concrete Triangle class derived from AbstractShape
class Triangle : public AbstractShape {
private:
    Vec3* vertexA = nullptr;
    Vec3* vertexB = nullptr;
    Vec3* vertexC = nullptr;
    Vec3* vertexANormal = nullptr;
    Vec3* vertexBNormal = nullptr;
    Vec3* vertexCNormal = nullptr;
    Vec2* textureCoordinateA = nullptr;
    Vec2* textureCoordinateB = nullptr;
    Vec2* textureCoordinateC = nullptr;

public:
    // Constructor
    Triangle(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC) : vertexA(vertexA), vertexB(vertexB), vertexC(vertexC) {}

    // Setters
    void setVertexANormal(Vec3* normal) { vertexANormal = normal; }
    void setVertexBNormal(Vec3* normal) { vertexBNormal = normal; }
    void setVertexCNormal(Vec3* normal) { vertexCNormal = normal; }
    void setTextureCoordinateA(Vec2* coord) { textureCoordinateA = coord; }
    void setTextureCoordinateB(Vec2* coord) { textureCoordinateB = coord; }
    void setTextureCoordinateC(Vec2* coord) { textureCoordinateC = coord; }

    // Other methods
    Vec3 calculateBarycentricCoordinates(const Vec3& intersectionPoint) const;

    // Override methods
    bool intersects(const Ray& ray, Vec3& intersectionPoint) const override;
    Vec3 getBoundingBoxMin() const override;
    Vec3 getBoundingBoxMax() const override;
    Vec2 calculateTextureCoordinate(const Vec3& intersectionPoint) const override;
};
