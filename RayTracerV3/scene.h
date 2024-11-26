#pragma once

#include <vector>

#include "color.h"
#include "shapes.h"
#include "lights.h"

#include "bvh.h"
#include "depthCue.h"

class Scene {
private:
    Color backgroundColor;
    std::vector<AbstractShape*> shapes;
    std::vector<AbstractLight*> lights;
    BVHNode* BVHRoot = nullptr;
    DepthCue* depthCue = nullptr;

public:
    // Constructors
    Scene() = default;
    Scene(Color backgroundColor, const std::vector<AbstractShape*>& shapes, const std::vector<AbstractLight*>& lights, const std::vector<Vec3>& vertices, const std::vector<Vec3>& vertexNormals, const std::vector<Vec2>& textureCoordinates, BVHNode* BVHRoot, DepthCue* depthCue)
        : backgroundColor(backgroundColor), shapes(shapes), lights(lights), BVHRoot(BVHRoot), depthCue(depthCue) {}

    // Destructor
    ~Scene() {
        for (auto shape : shapes) delete shape;
        for (auto light : lights) delete light;
        delete BVHRoot;
        delete depthCue;
    }

    // Getters
    const Color& getBackgroundColor() const { return this->backgroundColor; }
    const std::vector<AbstractShape*>& getShapes() const { return this->shapes; }
    const std::vector<AbstractLight*>& getLights() const { return this->lights; }

    const BVHNode* getBVHRoot() const { return this->BVHRoot; }
    const DepthCue* getDepthCue() const { return this->depthCue; }

    // Other methods
    const AbstractShape* findClosestIntersectedShape(const Ray& ray, Vec3& intersectionPoint) const;
};


class SceneBuilder {
private:
    Color backgroundColor;
    std::vector<AbstractShape*> shapes;
    std::vector<AbstractLight*> lights;

    std::shared_ptr<IMaterial> currentMaterial = nullptr; // Use shared_ptr for shared ownership
    std::shared_ptr<Texture> currentTexture = nullptr;    // Use shared_ptr for shared ownership
    std::shared_ptr<NormalMap> currentNormalMap = nullptr; // Use shared_ptr for shared ownership

    std::vector<Vec3> vertices;
    std::vector<Vec3> vertexNormals;
    std::vector<Vec2> textureCoordinates;

    BVHNode* BVHRoot = nullptr;         // Still raw pointer
    DepthCue* depthCue = nullptr;       // Still raw pointer

public:

    // Getters
    std::vector<AbstractShape*>& getShapes() { return this->shapes; }

    std::shared_ptr<IMaterial> getCurrentMaterial() const { return this->currentMaterial; }
    std::shared_ptr<Texture> getCurrentTexture() const { return this->currentTexture; }
    std::shared_ptr<NormalMap> getCurrentNormalMap() const { return this->currentNormalMap; }

    const std::vector<Vec3>& getVertices() const { return this->vertices; }
    const std::vector<Vec3>& getVertexNormals() const { return this->vertexNormals; }
    const std::vector<Vec2>& getTextureCoordinates() const { return this->textureCoordinates; }

    // Setters
    void addShape(AbstractShape* shape) { this->shapes.push_back(shape); }
    void addLight(AbstractLight* light) { this->lights.push_back(light); }
    void setBackgroundColor(const Color& color) { this->backgroundColor = color; }

    void setCurrentMaterial(const std::shared_ptr<IMaterial>& material) { this->currentMaterial = material; }
    void setCurrentTexture(const std::shared_ptr<Texture>& texture) { this->currentTexture = texture; }
    void setCurrentNormalMap(const std::shared_ptr<NormalMap>& normalMap) { this->currentNormalMap = normalMap; }

    void addVertex(const Vec3& vertex) { this->vertices.push_back(vertex); }
    void addVertexNormal(const Vec3& vertexNormal) { this->vertexNormals.push_back(vertexNormal); }
    void addTextureCoordinate(const Vec2& textureCoordinate) { this->textureCoordinates.push_back(textureCoordinate); }

    void setBVHRoot(BVHNode* root) { this->BVHRoot = root; }
    void setDepthCue(DepthCue* depthCue) { this->depthCue = depthCue; }

    // Build method
    Scene build() const { return Scene(backgroundColor, shapes, lights, vertices, vertexNormals, textureCoordinates, BVHRoot, depthCue); }
};