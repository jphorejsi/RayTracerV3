#include "shapeFactory.h"

#include <stdexcept>

// SphereFactory method implementations
AbstractShape* SphereFactory::createSphere(const Vec3& position, float radius, IMaterial* material) {
    if (radius <= 0) {
        throw std::runtime_error("Error: Radius must be positive.");
    }
    Sphere* sphere = new Sphere(position, radius);  // Initialize with only position and radius
    sphere->setMaterial(material ? material : new RGBMaterial(Color(1, 1, 1)));  // Set material, defaulting to white if null
    return sphere;
}

AbstractShape* SphereFactory::createTexturedSphere(const Vec3& position, float radius, IMaterial* material, Texture* texture) {
    if (radius <= 0) {
        throw std::runtime_error("Error: Radius must be positive.");
    }
    if (!texture) {
        throw std::runtime_error("Error: Texture must be provided for a textured shape.");
    }
    Sphere* sphere = new Sphere(position, radius);  // Initialize with only position and radius
    sphere->setMaterial(material ? material : new RGBMaterial(Color(1, 1, 1)));
    sphere->setTexture(texture);  // Set texture after construction
    return sphere;
}

AbstractShape* SphereFactory::createNormalMappedSphere(const Vec3& position, float radius, IMaterial* material, Texture* texture, NormalMap* normalMap) {
    if (radius <= 0) {
        throw std::runtime_error("Error: Radius must be positive.");
    }
    if (!texture) {
        throw std::runtime_error("Error: Texture must be provided for a normal mapped shape.");
    }
    if (!normalMap) {
        throw std::runtime_error("Error: Normal map must be provided for a normal mapped shape.");
    }
    Sphere* sphere = new Sphere(position, radius);  // Initialize with only position and radius
    sphere->setMaterial(material ? material : new RGBMaterial(Color(1, 1, 1)));
    sphere->setTexture(texture);
    sphere->setNormalMap(normalMap);  // Set normal map after construction
    return sphere;
}

//// TriangleFactory method implementations
//AbstractShape* TriangleFactory::createTriangle(Vec3& vertexA, Vec3& vertexB, Vec3& vertexC, IMaterial* material) {
//    if (!material) {
//        material = new RGBMaterial(Color(1, 1, 1));  // Default to white if no material provided
//    }
//    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);  // Initialize vertices
//    triangle->setMaterial(material);
//    return triangle;
//}
//
//AbstractShape* TriangleFactory::createTexturedTriangle(Vec3& vertexA, Vec2& textureCoordinateA, Vec3& vertexB, Vec2& textureCoordinateB, Vec3& vertexC, Vec2& textureCoordinateC, IMaterial* material, Texture* texture) {
//    if (!material) {
//        material = new RGBMaterial(Color(1, 1, 1));
//    }
//    if (!texture) {
//        throw std::runtime_error("Error: Texture must be provided for a textured shape.");
//    }
//    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
//    triangle->setMaterial(material);
//    triangle->setTexture(texture);
//    triangle->setTextureCoordinateA(textureCoordinateA);
//    triangle->setTextureCoordinateB(textureCoordinateB);
//    triangle->setTextureCoordinateC(textureCoordinateC);
//    return triangle;
//}
//
//AbstractShape* TriangleFactory::createNormalMappedTriangle(Vec3* vertexA, Vec3* vertexB, Vec3* vertexC, IMaterial* material, Texture* texture, NormalMap* normalMap) {
//    if (!material) {
//        material = new RGBMaterial(Color(1, 1, 1));
//    }
//    if (!texture) {
//        throw std::runtime_error("Error: Texture must be provided for a normal mapped shape.");
//    }
//    if (!normalMap) {
//        throw std::runtime_error("Error: Normal map must be provided for a normal mapped shape.");
//    }
//    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
//    triangle->setMaterial(material);
//    triangle->setTexture(texture);
//    triangle->setNormalMap(normalMap);
//    return triangle;
//}
//
//AbstractShape* TriangleFactory::createSmoothShadedTriangle(Vec3* vertexA, Vec3* vertexANormal, Vec3* vertexB, Vec3* vertexBNormal, Vec3* vertexC, Vec3* vertexCNormal, IMaterial* material) {
//    if (!material) {
//        material = new RGBMaterial(Color(1, 1, 1));
//    }
//    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
//    triangle->setMaterial(material);
//    triangle->setVertexANormal(vertexANormal);
//    triangle->setVertexBNormal(vertexBNormal);
//    triangle->setVertexCNormal(vertexCNormal);
//    return triangle;
//}
//
//AbstractShape* TriangleFactory::createSmoothShadedTexturedTriangle(Vec3* vertexA, Vec2* textureCoordinateA, Vec3* vertexANormal, Vec3* vertexB, Vec2* textureCoordinateB, Vec3* vertexBNormal, Vec3* vertexC, Vec2* textureCoordinateC, Vec3* vertexCNormal, IMaterial* material, Texture* texture) {
//    if (!material) {
//        material = new RGBMaterial(Color(1, 1, 1));
//    }
//    if (!texture) {
//        throw std::runtime_error("Error: Texture must be provided for a normal mapped shape.");
//    }
//    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
//    triangle->setMaterial(material);
//    triangle->setTexture(texture);
//    triangle->setTextureCoordinateA(textureCoordinateA);
//    triangle->setTextureCoordinateA(textureCoordinateB);
//    triangle->setTextureCoordinateA(textureCoordinateC);
//    triangle->setVertexANormal(vertexANormal);
//    triangle->setVertexBNormal(vertexBNormal);
//    triangle->setVertexCNormal(vertexCNormal);
//    return triangle;
//}

AbstractShape* TriangleFactory::createTriangle(const Vec3* vertexA, const Vec3* vertexB, const Vec3* vertexC, IMaterial* material) {
    if (!material) {
        material = new RGBMaterial(Color(1, 1, 1));  // Default to white if no material provided
    }
    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
    triangle->setMaterial(material);
    return triangle;
}

AbstractShape* TriangleFactory::createTexturedTriangle(const Vec3* vertexA, const Vec2* textureCoordinateA,
    const Vec3* vertexB, const Vec2* textureCoordinateB,
    const Vec3* vertexC, const Vec2* textureCoordinateC,
    IMaterial* material, Texture* texture) {
    if (!material) {
        material = new RGBMaterial(Color(1, 1, 1));
    }
    if (!texture) {
        throw std::runtime_error("Error: Texture must be provided for a textured shape.");
    }
    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
    triangle->setMaterial(material);
    triangle->setTexture(texture);
    triangle->setTextureCoordinateA(textureCoordinateA);
    triangle->setTextureCoordinateB(textureCoordinateB);
    triangle->setTextureCoordinateC(textureCoordinateC);
    return triangle;
}

AbstractShape* TriangleFactory::createSmoothShadedTriangle(const Vec3* vertexA, const Vec3* vertexANormal,
    const Vec3* vertexB, const Vec3* vertexBNormal,
    const Vec3* vertexC, const Vec3* vertexCNormal,
    IMaterial* material) {
    if (!material) {
        material = new RGBMaterial(Color(1, 1, 1));
    }
    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
    triangle->setMaterial(material);
    triangle->setVertexANormal(vertexANormal);
    triangle->setVertexBNormal(vertexBNormal);
    triangle->setVertexCNormal(vertexCNormal);
    return triangle;
}

AbstractShape* TriangleFactory::createSmoothShadedTexturedTriangle(const Vec3* vertexA, const Vec2* textureCoordinateA, const Vec3* vertexANormal,
    const Vec3* vertexB, const Vec2* textureCoordinateB, const Vec3* vertexBNormal,
    const Vec3* vertexC, const Vec2* textureCoordinateC, const Vec3* vertexCNormal,
    IMaterial* material, Texture* texture) {
    if (!material) {
        material = new RGBMaterial(Color(1, 1, 1));
    }
    if (!texture) {
        throw std::runtime_error("Error: Texture must be provided for a textured shape.");
    }
    Triangle* triangle = new Triangle(vertexA, vertexB, vertexC);
    triangle->setMaterial(material);
    triangle->setTexture(texture);
    triangle->setTextureCoordinateA(textureCoordinateA);
    triangle->setTextureCoordinateB(textureCoordinateB);
    triangle->setTextureCoordinateC(textureCoordinateC);
    triangle->setVertexANormal(vertexANormal);
    triangle->setVertexBNormal(vertexBNormal);
    triangle->setVertexCNormal(vertexCNormal);
    return triangle;
}
