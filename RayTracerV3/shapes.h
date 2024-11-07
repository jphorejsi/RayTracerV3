#pragma once

#include <string>
#include <memory>
#include "vec.h"
#include "materials.h"
#include "texture.h"
#include "ray.h"
#include "color.h"

// Forward declaration of Scene class
class Scene;

// Abstract base class for shapes
class IShape {
public:
    virtual ~IShape() = default;

    // Pure virtual getters
    virtual IMaterial* getMaterial() const = 0;
    virtual Texture* getTexture() const = 0;
    virtual NormalMap* getNormalMap() const = 0;

    // pure virutual setters
    virtual void setMaterial(IMaterial* mat) = 0;
    virtual void setTexture(Texture* tex) = 0;
    virtual void setNormalMap(NormalMap* nMap) = 0;

    // Pure virtual methods
    virtual bool intersects(const Ray& ray, Vec3& intersectionPoint) const = 0;
    virtual Vec3 getBoundingBoxMin() const = 0;
    virtual Vec3 getBoundingBoxMax() const = 0;
    virtual Vec2 calculateTextureCoordinate(const Vec3& intersectionPoint) const = 0;
};

// Concrete sphere class
class Sphere : public IShape {
private:
    float radius;
    Vec3 position;

    IMaterial* material = nullptr;
    Texture* texture = nullptr;
    NormalMap* normalMap = nullptr;

public:
    // Constructor
    Sphere(const Vec3& position, float radius) : position(position), radius(radius) {}

    // Overrides for getters
    IMaterial* getMaterial() const override { return this->material; }
    Texture* getTexture() const override { return this->texture; }
    NormalMap* getNormalMap() const override { return this->normalMap; }

    // Overrides for setters
    void setMaterial(IMaterial* mat) override { this->material = mat; }
    void setTexture(Texture* tex) override { this->texture = tex; }
    void setNormalMap(NormalMap* nMap) override { this->normalMap = nMap; }

    // Overrides methods
    bool intersects(const Ray& ray, Vec3& intersectionPoint) const override;
    Vec3 getBoundingBoxMin() const override;
    Vec3 getBoundingBoxMax() const override;
    virtual Vec2 calculateTextureCoordinate(const Vec3& intersectionPoint) const override;
};

// Concrete triangle class
class Triangle : public IShape {
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

    IMaterial* material = nullptr;
    Texture* texture = nullptr;
    NormalMap* normalMap = nullptr;

public:
    // Constructor
    Triangle(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC) : vertexA(vertexA), vertexB(vertexB), vertexC(vertexC) {}

    // Setters
    void setVertexANormal(Vec3* normal) { this->vertexANormal = normal; }
    void setVertexBNormal(Vec3* normal) { this->vertexBNormal = normal; }
    void setVertexCNormal(Vec3* normal) { this->vertexCNormal = normal; }
    void setTextureCoordinateA(Vec2* coord) { this->textureCoordinateA = coord; }
    void setTextureCoordinateB(Vec2* coord) { this->textureCoordinateB = coord; }
    void setTextureCoordinateC(Vec2* coord) { this->textureCoordinateC = coord; }

    // Other methods
    Vec3 calculateBarycentricCoordinates(const Vec3& intersectionPoint) const;

    // Overrides getters
    IMaterial* getMaterial() const override { return this->material; }
    Texture* getTexture() const override { return this->texture; }
    NormalMap* getNormalMap() const override { return this->normalMap; }

    // Override setters
    void setMaterial(IMaterial* mat) override { this->material = mat; }
    void setTexture(Texture* tex) override { this->texture = tex; }
    void setNormalMap(NormalMap* nMap) override { this->normalMap = nMap; }

    // Overrides methods
    bool intersects(const Ray& ray, Vec3& intersectionPoint) const override;
    Vec3 getBoundingBoxMin() const override;
    Vec3 getBoundingBoxMax() const override;
    virtual Vec2 calculateTextureCoordinate(const Vec3& intersectionPoint) const override;
};