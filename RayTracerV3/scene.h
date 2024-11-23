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

    std::vector<IMaterial*> materials;
    std::vector<Texture*> textures;
    std::vector<NormalMap*> normalMaps;
    
    std::vector<Vec3> vertices;
    std::vector<Vec3> vertexNormals;
    std::vector<Vec2> textureCoordinates;

    BVHNode* BVHRoot = nullptr;
    DepthCue* depthCue = nullptr;

public:
    // Constructors
    Scene() = default;
    Scene(Color backgroundColor, const std::vector<AbstractShape*>& shapes, const std::vector<AbstractLight*>& lights, const std::vector<IMaterial*>& materials, const std::vector<Texture*>& textures, const std::vector<NormalMap*>& normalMaps, const std::vector<Vec3>& vertices, const std::vector<Vec3>& vertexNormals, const std::vector<Vec2>& textureCoordinates, BVHNode* BVHRoot, DepthCue* depthCue)
        : backgroundColor(backgroundColor), shapes(shapes), lights(lights), materials(materials), textures(textures), normalMaps(normalMaps), vertices(vertices), vertexNormals(vertexNormals), textureCoordinates(textureCoordinates), BVHRoot(BVHRoot), depthCue(depthCue) {}

    // Destructor
    ~Scene() {
        for (auto shape : shapes) delete shape;
        for (auto light : lights) delete light;
        for (auto material : materials) delete material;
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

    std::vector<IMaterial*> materials;
    std::vector<Texture*> textures;
    std::vector<NormalMap*> normalMaps;

    std::vector<Vec3> vertices;
    std::vector<Vec3> vertexNormals;
    std::vector<Vec2> textureCoordinates;

    BVHNode* BVHroot = nullptr;
    DepthCue* depthCue = nullptr;
public:
    // Getters
    std::vector<AbstractShape*>& getShapes() { return this->shapes; }

    const std::vector<IMaterial*>& getMaterials() const { return this->materials; }
    const std::vector<Texture*>& getTextures() const { return this->textures; }
    const std::vector<NormalMap*>& getNormalMaps() const { return this->normalMaps; }

    const std::vector<Vec3>& getVertices() const { return this->vertices; }
    const std::vector<Vec3>& getVertexNormals() const { return this->vertexNormals; }
    const std::vector<Vec2>& getTextureCoordinates() const { return this->textureCoordinates; }

    // Setters
    void addShape(AbstractShape* shape) { this->shapes.push_back(shape); }
    void addLight(AbstractLight* light) { this->lights.push_back(light); }
    void setBackgroundColor(const Color& color) { this->backgroundColor = color; }

    void addMaterial(IMaterial* material) { this->materials.push_back(material); }
    void addTexture(Texture* texture) { this->textures.push_back(texture); }
    void addNormalMap(NormalMap* normalMap) { this->normalMaps.push_back(normalMap); }

    void addVertex(Vec3 vertex) { this->vertices.push_back(vertex); }
    void addVertexNormal(Vec3 vertexNormal) { this->vertexNormals.push_back(vertexNormal); }
    void addTextureCoordinate(Vec2 textureCoordinate) { this->textureCoordinates.push_back(textureCoordinate); }

    void setBVHRoot(BVHNode* root) { this->BVHroot = root; }
    void setDepthCue(DepthCue* depthCue) { this->depthCue = depthCue; }

    // Build method
    Scene build() const { return Scene(backgroundColor, shapes, lights, materials, textures, normalMaps, vertices, vertexNormals, textureCoordinates, BVHroot, depthCue); }
};