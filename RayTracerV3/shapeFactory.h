#pragma once

#include "shapes.h"
#include "vec.h"
#include "materials.h"
#include "texture.h"

class SphereFactory {
public:
    static AbstractShape* createSphere(const Vec3& position, double radius);
    static AbstractShape* createMaterialSphere(const Vec3& position, double radius, Material& material);
    static AbstractShape* createTexturedSphere(const Vec3& position, double radius, Material& material, Texture& texture);
    static AbstractShape* createNormalMappedSphere(const Vec3& position, double radius, Material& material, Texture& texture, NormalMap& normalMap);
};


class TriangleFactory {
public:
    static AbstractShape* createTriangle(const Vec3& vertexA, const Vec3& vertexB, const Vec3& vertexC);
    static AbstractShape* createMaterialTriangle(const Vec3& vertexA, const Vec3& vertexB, const Vec3& vertexC, Material& material);
    static AbstractShape* createSmoothShadedMaterialTriangle(const Vec3& vertexA, const Vec3& vertexB, const Vec3& vertexC, const Vec3& vertexANormal, const Vec3& vertexBNormal, const Vec3& vertexCNormal, Material& material);
    static AbstractShape* createTexturedTriangle(const Vec3& vertexA, const Vec2& textureCoordinateA, const Vec3& vertexB, const Vec2& textureCoordinateB, const Vec3& vertexC, const Vec2& textureCoordinateC, Material& material, Texture& texture);
    static AbstractShape* createNormalMappedTriangle(const Vec3& vertexA, const Vec2& textureCoordinateA, const Vec3& vertexANormal, const Vec3& vertexB, const Vec2& textureCoordinateB, const Vec3& vertexBNormal, const Vec3& vertexC, const Vec2& textureCoordinateC, const Vec3& vertexCNormal, Material& material, Texture& texture, NormalMap& normalMap);
    static AbstractShape* createSmoothShadedTexturedTriangle(const Vec3& vertexA, const Vec2& textureCoordinateA, const Vec3& vertexANormal, const Vec3& vertexB, const Vec2& textureCoordinateB, const Vec3& vertexBNormal, const Vec3& vertexC, const Vec2& textureCoordinateC, const Vec3& vertexCNormal, Material& material, Texture& texture);
};
