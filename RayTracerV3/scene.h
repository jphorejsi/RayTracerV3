#pragma once

#include "color.h"
#include "shapes.h"
#include "lights.h"
#include "bvh.h"
#include <vector>

class Scene {
private:
    std::vector<AbstractShape*> shapes;
    std::vector<AbstractLight*> lights;
    std::vector<IMaterial*> materials;
    std::vector<Texture*> textures;
    std::vector<NormalMap*> normalMaps;
    BVHNode* root;

    Color backgroundColor;
    std::vector<Vec3> vertices;
    std::vector<Vec3> vertexNormals;
    std::vector<Vec2> textureCoordinates;
public:
    // Constructors
    Scene() = default;
    Scene(Color backgroundColor, const std::vector<AbstractShape*>& shapes, const std::vector<AbstractLight*>& lights, BVHNode* root, const std::vector<IMaterial*>& materials, const std::vector<Texture*>& textures, const std::vector<NormalMap*>& normalMaps, const std::vector<Vec3>& vertices, const std::vector<Vec3>& vertexNormals, const std::vector<Vec2>& textureCoordinates)
        : backgroundColor(backgroundColor), shapes(shapes), lights(lights), root(root), materials(materials), textures(textures), normalMaps(normalMaps), vertices(vertices), vertexNormals(vertexNormals), textureCoordinates(textureCoordinates) {}

    // Destructor
    ~Scene() {
        for (auto shape : shapes) delete shape;
        for (auto light : lights) delete light;
        for (auto material : materials) delete material;
        delete root;
    }

    // Getters
    const Color& getBackgroundColor() const { return backgroundColor; }
    const std::vector<AbstractLight*>& getLights() const { return lights; }
    const std::vector<AbstractShape*>& getShapes() const { return shapes; }
    const BVHNode* getBVHRoot() const { return root; }

    // Other methods
    const AbstractShape* findClosestIntersectedShape(const Ray& ray, Vec3& intersectionPoint) const;
};


class SceneBuilder {
private:
    std::vector<AbstractShape*> shapes;
    std::vector<AbstractLight*> lights;
    std::vector<IMaterial*> materials;
    std::vector<Texture*> textures;
    std::vector<NormalMap*> normalMaps;
    BVHNode* root = nullptr;

    Color backgroundColor;
    std::vector<Vec3> vertices;
    std::vector<Vec3> vertexNormals;
    std::vector<Vec2> textureCoordinates;
public:
    // Getters
    const Color& getBackgroundColor() const { return backgroundColor; }
    const std::vector<AbstractLight*>& getLights() const { return lights; }
    std::vector<AbstractShape*>& getShapes() { return shapes; }
    const BVHNode* getBVHRoot() const { return root; }

    const std::vector<IMaterial*>& getMaterials() const { return materials; }
    const std::vector<Texture*>& getTextures() const { return textures; }
    const std::vector<NormalMap*>& getNormalMaps() const { return normalMaps; }
    const std::vector<Vec3>& getVertices() const { return vertices; }
    const std::vector<Vec3>& getVertexNormals() const { return vertexNormals; }
    const std::vector<Vec2>& getTextureCoordinates() const { return textureCoordinates; }

    // Setters
    void setBackgroundColor(const Color& color) {
        // Check that color components are within the valid range
        if (color.getR() < 0.0f || color.getR() > 1.0f ||
            color.getG() < 0.0f || color.getG() > 1.0f ||
            color.getB() < 0.0f || color.getB() > 1.0f) {
            throw std::invalid_argument("Error: Background Color's values must be between 0 and 1.");
        }
        this->backgroundColor = color;
    }

    void addShape(AbstractShape* shape) { this->shapes.push_back(shape); }
    void addLight(AbstractLight* light) { this->lights.push_back(light); }
    void setBVHRoot(BVHNode* root) { this->root = root; }
    void addMaterial(IMaterial* material) { this->materials.push_back(material); }
    void addTexture(Texture* texture) { this->textures.push_back(texture); }
    void addVertex(Vec3 vertex) { this->vertices.push_back(vertex); }
    void addVertexNormal(Vec3 vertexNormal) { this->vertexNormals.push_back(vertexNormal); }
    void addTextureCoordinate(Vec2 textureCoordinate) { this->textureCoordinates.push_back(textureCoordinate); }

    // Build method
    Scene build() const {
        return Scene(backgroundColor, shapes, lights, root, materials, textures, normalMaps, vertices, vertexNormals, textureCoordinates);
    }
};