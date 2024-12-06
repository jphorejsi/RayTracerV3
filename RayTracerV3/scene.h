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

    Material* currentMaterial = nullptr;
    Texture* currentTexture = nullptr;
    NormalMap* currentNormalMap = nullptr;
    BVHNode* BVHRoot = nullptr;
    DepthCue* depthCue = nullptr;

    std::vector<Vec3> vertices;
    std::vector<Vec3> vertexNormals;
    std::vector<Vec2> textureCoordinates;

public:

    // Getters
    std::vector<AbstractShape*>& getShapes() { return this->shapes; }

    Material* getCurrentMaterial() const { return this->currentMaterial; }
    Texture* getCurrentTexture() const { return this->currentTexture; }
    NormalMap* getCurrentNormalMap() const { return this->currentNormalMap; }

    const std::vector<Vec3>& getVertices() const { return this->vertices; }
    const std::vector<Vec3>& getVertexNormals() const { return this->vertexNormals; }
    const std::vector<Vec2>& getTextureCoordinates() const { return this->textureCoordinates; }

    // Setters
    void setBackgroundColor(const Color& color) { this->backgroundColor = color; }
    void setCurrentMaterial(Material* material) { 
        delete currentMaterial;
        this->currentMaterial = material;
    }
    void setCurrentTexture(Texture* texture) { this->currentTexture = texture; }
    void setCurrentNormalMap(NormalMap* normalMap) { this->currentNormalMap = normalMap; }
    void setBVHRoot(BVHNode* root) { this->BVHRoot = root; }
    void setDepthCue(DepthCue* depthCue) { this->depthCue = depthCue; }

    // Add methods
    void addShape(AbstractShape* shape) { this->shapes.push_back(shape); }
    void addLight(AbstractLight* light) { this->lights.push_back(light); }
    void addVertex(const Vec3& vertex) { this->vertices.push_back(vertex); }
    void addVertexNormal(const Vec3& vertexNormal) { this->vertexNormals.push_back(vertexNormal); }
    void addTextureCoordinate(const Vec2& textureCoordinate) { this->textureCoordinates.push_back(textureCoordinate); }

    // Build method
    Scene build() const { return Scene(backgroundColor, shapes, lights, vertices, vertexNormals, textureCoordinates, BVHRoot, depthCue); }
};