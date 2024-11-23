#pragma once

#include "vec.h"
#include "materials.h"
#include "texture.h"
#include "ray.h"
#include "color.h"
#include "aabb.h"

// Abstract base class for shapes
class AbstractShape {
protected:
    IMaterial* material = nullptr;
    Texture* texture = nullptr;
    NormalMap* normalMap = nullptr;
    AABB* aabb = nullptr;

public:
    virtual ~AbstractShape() { delete aabb; }

    // Getters
    IMaterial* getMaterial() const { return material; }
    Texture* getTexture() const { return texture; }
    NormalMap* getNormalMap() const { return normalMap; }
    const AABB* getAABB() const { return aabb; }

    // Setters
    void setMaterial(IMaterial* mat) { material = mat; }
    void setTexture(Texture* tex) { texture = tex; }
    void setNormalMap(NormalMap* nMap) { normalMap = nMap; }

    // Pure virtual methods
    virtual bool intersects(const Ray& ray, Vec3& intersectionPoint) const = 0;
    virtual Vec3 getBoundingBoxMin() const = 0;
    virtual Vec3 getBoundingBoxMax() const = 0;
    virtual Vec2 getTextureCoordinate(const Vec3& intersectionPoint) const = 0;
    virtual Vec3 getNormal(const Vec3& intersectionPoint) const = 0;
    virtual Vec3 getCentroid() const = 0;


    // Other methods
    virtual void computeAABB() {
        delete aabb;
        aabb = new AABB(getBoundingBoxMin(), getBoundingBoxMax());
    }
};

// Concrete Sphere class derived from AbstractShape
class Sphere : public AbstractShape {
private:
    double radius;
    Vec3 position;

public:
    // Constructor
    Sphere(const Vec3& position, double radius) : position(position), radius(radius) {
        computeAABB();
    }

    // Override methods
    bool intersects(const Ray& ray, Vec3& intersectionPoint) const override;
    Vec3 getBoundingBoxMin() const override;
    Vec3 getBoundingBoxMax() const override;
    Vec2 getTextureCoordinate(const Vec3& intersectionPoint) const override;
    Vec3 getNormal(const Vec3& intersectionPoint) const override;
    Vec3 getCentroid() const { return position; }

};

// Concrete Triangle class derived from AbstractShape
class Triangle : public AbstractShape {
private:
    const Vec3* vertexA = nullptr;
    const Vec3* vertexB = nullptr;
    const Vec3* vertexC = nullptr;
    const Vec3* vertexANormal = nullptr;
    const Vec3* vertexBNormal = nullptr;
    const Vec3* vertexCNormal = nullptr;
    const Vec2* textureCoordinateA = nullptr;
    const Vec2* textureCoordinateB = nullptr;
    const Vec2* textureCoordinateC = nullptr;

public:
    // Constructor
    Triangle(const Vec3* vertexA, const Vec3* vertexB, const Vec3* vertexC) : vertexA(vertexA), vertexB(vertexB), vertexC(vertexC) {
        computeAABB();
    }

    // Setters
    void setVertexANormal(const Vec3* normal) { vertexANormal = normal; }
    void setVertexBNormal(const Vec3* normal) { vertexBNormal = normal; }
    void setVertexCNormal(const Vec3* normal) { vertexCNormal = normal; }
    void setTextureCoordinateA(const Vec2* coord) { textureCoordinateA = coord; }
    void setTextureCoordinateB(const Vec2* coord) { textureCoordinateB = coord; }
    void setTextureCoordinateC(const Vec2* coord) { textureCoordinateC = coord; }

    // Other methods
    Vec3 getBarycentricCoordinates(const Vec3& intersectionPoint) const;

    // Override methods
    bool intersects(const Ray& ray, Vec3& intersectionPoint) const override;
    Vec3 getBoundingBoxMin() const override;
    Vec3 getBoundingBoxMax() const override;
    Vec2 getTextureCoordinate(const Vec3& intersectionPoint) const override;
    Vec3 getNormal(const Vec3& intersectionPoint) const override;
    Vec3 getCentroid() const { return (*vertexA + *vertexB + *vertexC) / 3.0f; }

};
