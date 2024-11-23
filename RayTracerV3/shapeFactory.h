#pragma once

#include "shapes.h"
#include "vec.h"
#include "materials.h"
#include "texture.h"

class SphereFactory {
public:
    static AbstractShape* createSphere(const Vec3& position, double radius, IMaterial* material);
    static AbstractShape* createTexturedSphere(const Vec3& position, double radius, IMaterial* material, Texture* texture);
    static AbstractShape* createNormalMappedSphere(const Vec3& position, double radius, IMaterial* material, Texture* texture, NormalMap* normalMap);
};


//class TriangleFactory {
//public:
//    static AbstractShape* createTriangle(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, IMaterial* material);
//    static AbstractShape* createTexturedTriangle(Vec3* vertexA, Vec2* textureCoordinateA, Vec3* vertexB, Vec2* textureCoordinateB, Vec3* vertexC, Vec2* textureCoordinateC, IMaterial* material, Texture* texture);
//    static AbstractShape* createNormalMappedTriangle(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, IMaterial* material, Texture* texture, NormalMap* normalMap);
//
//    static AbstractShape* createSmoothShadedTriangle(Vec3* vertexA, Vec3* vertexNormalA, Vec3* vertexB, Vec3* vertexNormalB, Vec3* vertexC, Vec3* vertexNormalC, IMaterial* material);
//    static AbstractShape* createSmoothShadedTexturedTriangle(Vec3* vertexA, Vec2* textureCoordinateA, Vec3* vertexNormalA, Vec3* vertexB, Vec2* textureCoordinateB, Vec3* vertexNormalB, Vec3* vertexC, Vec2* textureCoordinateC, Vec3* vertexNormalC, IMaterial* material, Texture* texture);
//};

class TriangleFactory {
public:
    static AbstractShape* createTriangle(const Vec3* vertexA, const Vec3* vertexB, const Vec3* vertexC, IMaterial* material);
    static AbstractShape* createTexturedTriangle(const Vec3* vertexA, const Vec2* textureCoordinateA, const Vec3* vertexB, const Vec2* textureCoordinateB, const Vec3* vertexC, const Vec2* textureCoordinateC, IMaterial* material, Texture* texture);
    static AbstractShape* createSmoothShadedTriangle(const Vec3* vertexA, const Vec3* vertexANormal, const Vec3* vertexB, const Vec3* vertexBNormal, const Vec3* vertexC, const Vec3* vertexCNormal, IMaterial* material);
    static AbstractShape* createSmoothShadedTexturedTriangle(const Vec3* vertexA, const Vec2* textureCoordinateA, const Vec3* vertexANormal, const Vec3* vertexB, const Vec2* textureCoordinateB, const Vec3* vertexBNormal, const Vec3* vertexC, const Vec2* textureCoordinateC, const Vec3* vertexCNormal, IMaterial* material, Texture* texture);
};
