#pragma once

#include "shapes.h"
#include "vec.h"
#include "materials.h"
#include "texture.h"

class SphereFactory {
public:
    static IShape* createSphere(const Vec3& position, float radius, IMaterial* material);
    static IShape* createTexturedSphere(const Vec3& position, float radius, IMaterial* material, Texture* texture);
    static IShape* createNormalMappedSphere(const Vec3& position, float radius, IMaterial* material, Texture* texture, NormalMap* normalMap);
};


class TriangleFactory {
public:
    static IShape* createTriangle(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, IMaterial* material);
    static IShape* createTexturedTriangle(Vec3* vertexA, Vec2* textureCoordinateA, Vec3* vertexB, Vec2* textureCoordinateB, Vec3* vertexC, Vec2* textureCoordinateC, IMaterial* material, Texture* texture);
    static IShape* createNormalMappedTriangle(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, IMaterial* material, Texture* texture, NormalMap* normalMap);

    static IShape* createSmoothShadedTriangle(Vec3* vertexA, Vec3* vertexNormalA, Vec3* vertexB, Vec3* vertexNormalB, Vec3* vertexC, Vec3* vertexNormalC, IMaterial* material);
    static IShape* createSmoothShadedTexturedTriangle(Vec3* vertexA, Vec2* textureCoordinateA, Vec3* vertexNormalA, Vec3* vertexB, Vec2* textureCoordinateB, Vec3* vertexNormalB, Vec3* vertexC, Vec2* textureCoordinateC, Vec3* vertexNormalC, IMaterial* material, Texture* texture);
};