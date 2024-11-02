#pragma once

#include <string>
#include "vec.h"
#include "material.h"
#include "texture.h"
#include "ray.h"

// Abstract base class for shapes
class IShape {
public:
    virtual ~IShape() = default;
    virtual bool intersects(const Ray& ray) const = 0;
    virtual Vec3 getBoundingBoxMin() const = 0;
    virtual Vec3 getBoundingBoxMax() const = 0;
};

// Concrete sphere class
class Sphere : public IShape {
public:
    float radius;
    Vec3 position;

    IMaterial* material = nullptr;
    Texture* texture = nullptr;
    NormalMap* normalMap = nullptr;

public:
    // Constructor
    Sphere(const Vec3& position, float radius) : position(position), radius(radius) {}

    // Getters
    IMaterial* getMaterial() const { return this->material; }
    Texture* getTexture() const { return this->texture; }
    NormalMap* getNormalMap() const { return this->normalMap; }

    // Setters
    void setMaterial(IMaterial* mat) { this->material = mat; }
    void setTexture(Texture* tex) { this->texture = tex; }
    void setNormalMap(NormalMap* nMap) { this->normalMap = nMap; }

    // Overrides
    bool intersects(const Ray& ray) const override;
    Vec3 getBoundingBoxMin() const override;
    Vec3 getBoundingBoxMax() const override;
};

// Concrete triangle class
class Triangle : public IShape {
private:
    Vec3* vertexA = nullptr;
    Vec3* vertexB = nullptr;
    Vec3* vertexC = nullptr;
    Vec3* normalVertexA = nullptr;
    Vec3* normalVertexB = nullptr;
    Vec3* normalVertexC = nullptr;
    Vec2* textureCoordinateA = nullptr;
    Vec2* textureCoordinateB = nullptr;
    Vec2* textureCoordinateC = nullptr;

    IMaterial* material = nullptr;
    Texture* texture = nullptr;
    NormalMap* normalMap = nullptr;

public:
    // Constructor
    Triangle(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC) : vertexA(vertexA), vertexB(vertexB), vertexC(vertexC) {}

    ~Triangle() {
        delete material;
        delete texture;
        delete normalMap;
    }

    // Getters
    IMaterial* getMaterial() const { return this->material; }
    Texture* getTexture() const { return this->texture; }
    NormalMap* getNormalMap() const { return this->normalMap; }

    // Setters
    void setMaterial(IMaterial* mat) { this->material = mat; }
    void setTexture(Texture* tex) { this->texture = tex; }
    void setNormalMap(NormalMap* nMap) { this->normalMap = nMap; }

    // Overrides
    bool intersects(const Ray& ray) const override;
    Vec3 getBoundingBoxMin() const override;
    Vec3 getBoundingBoxMax() const override;
};

class SphereFactory {
public:
    static IShape* createShape(const Vec3& position, float radius, IMaterial* material = nullptr);
    static IShape* createTexturedShape(const Vec3& position, float radius, IMaterial* material, Texture* texture);
    static IShape* createNormalMappedShape(const Vec3& position, float radius, IMaterial* material, Texture* texture, NormalMap* normalMap);
};


class TriangleFactory {
public:
    static IShape* createShape(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, IMaterial* material = nullptr);
    static IShape* createTexturedShape(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, IMaterial* material, Texture* texture);
    static IShape* createNormalMappedShape(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, IMaterial* material, Texture* texture, NormalMap* normalMap);
};
