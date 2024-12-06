#include "shapeFactory.h"
#include <stdexcept>

// SphereFactory Implementation
AbstractShape* SphereFactory::createSphere(const Vec3& position, double radius) {
    Sphere* sphere = new Sphere(position, radius);
    return sphere;
}

AbstractShape* SphereFactory::createMaterialSphere(const Vec3& position, double radius, Material& material) {
    Sphere* sphere = new Sphere(position, radius);
    sphere->setMaterial(&material);
    return sphere;
}

AbstractShape* SphereFactory::createTexturedSphere(const Vec3& position, double radius, Material& material, Texture& texture) {
    Sphere* sphere = new Sphere(position, radius);
    sphere->setMaterial(&material);
    sphere->setTexture(&texture);
    return sphere;
}

AbstractShape* SphereFactory::createNormalMappedSphere(const Vec3& position, double radius, Material& material, Texture& texture, NormalMap& normalMap) {
    Sphere* sphere = new Sphere(position, radius);
    sphere->setMaterial(&material);
    sphere->setTexture(&texture);
    sphere->setNormalMap(&normalMap);
    return sphere;
}

AbstractShape* TriangleFactory::createTriangle(const Vec3& vertexA, const Vec3& vertexB, const Vec3& vertexC) {
    return new Triangle(vertexA, vertexB, vertexC);
}

AbstractShape* TriangleFactory::createMaterialTriangle(const Vec3& vertexA, const Vec3& vertexB, const Vec3& vertexC, Material& material) {
    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
    triangle->setMaterial(&material);
    return triangle;
}

AbstractShape* TriangleFactory::createSmoothShadedMaterialTriangle(const Vec3& vertexA, const Vec3& vertexB, const Vec3& vertexC, const Vec3& vertexANormal, const Vec3& vertexBNormal, const Vec3& vertexCNormal, Material& material) {
    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
    triangle->setMaterial(&material);
    triangle->setVertexANormal(&vertexANormal);
    triangle->setVertexBNormal(&vertexBNormal);
    triangle->setVertexCNormal(&vertexCNormal);
    return triangle;
}

AbstractShape* TriangleFactory::createTexturedTriangle(const Vec3& vertexA, const Vec2& textureCoordinateA, const Vec3& vertexB, const Vec2& textureCoordinateB, const Vec3& vertexC, const Vec2& textureCoordinateC, Material& material, Texture& texture) {
    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
    triangle->setMaterial(&material);
    triangle->setTexture(&texture);
    triangle->setTextureCoordinateA(&textureCoordinateA);
    triangle->setTextureCoordinateB(&textureCoordinateB);
    triangle->setTextureCoordinateC(&textureCoordinateC);
    return triangle;
}

AbstractShape* TriangleFactory::createSmoothShadedTexturedTriangle(const Vec3& vertexA, const Vec2& textureCoordinateA, const Vec3& vertexANormal, const Vec3& vertexB, const Vec2& textureCoordinateB, const Vec3& vertexBNormal, const Vec3& vertexC, const Vec2& textureCoordinateC, const Vec3& vertexCNormal, Material& material, Texture& texture) {
    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
    triangle->setMaterial(&material);
    triangle->setTexture(&texture);
    triangle->setTextureCoordinateA(&textureCoordinateA);
    triangle->setTextureCoordinateB(&textureCoordinateB);
    triangle->setTextureCoordinateC(&textureCoordinateC);
    triangle->setVertexANormal(&vertexANormal);
    triangle->setVertexBNormal(&vertexBNormal);
    triangle->setVertexCNormal(&vertexCNormal);
    return triangle;
}

AbstractShape* TriangleFactory::createNormalMappedTriangle(
    const Vec3& vertexA, const Vec2& textureCoordinateA, const Vec3& vertexANormal,
    const Vec3& vertexB, const Vec2& textureCoordinateB, const Vec3& vertexBNormal,
    const Vec3& vertexC, const Vec2& textureCoordinateC, const Vec3& vertexCNormal,
    Material& material, Texture& texture, NormalMap& normalMap) {
    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
    triangle->setMaterial(&material);
    triangle->setTexture(&texture);
    triangle->setNormalMap(&normalMap);
    triangle->setVertexANormal(&vertexANormal);
    triangle->setVertexBNormal(&vertexBNormal);
    triangle->setVertexCNormal(&vertexCNormal);
    triangle->setTextureCoordinateA(&textureCoordinateA);
    triangle->setTextureCoordinateB(&textureCoordinateB);
    triangle->setTextureCoordinateC(&textureCoordinateC);
    return triangle;
}

