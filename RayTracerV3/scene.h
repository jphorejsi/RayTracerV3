#pragma once

#include <vector>
#include "color.h"
#include "shapes.h"
#include "lights.h"
#include "kdtree.h"

class SceneType {
    ColorType backgroundColor;
    std::vector<ShapeType*> shapes;
    std::vector<LightType*> lights;
    KDTreeNode* root;

    std::vector<MaterialType*> materials;
    std::vector<TextureType*> textures;
    std::vector<Vec3*> vertices;
    std::vector<Vec3*> normalVectors;
    std::vector<Vec2*> textureCoordinates;

public:
    // Constructor
    SceneType() : root(nullptr) {}

    // Destructor
    ~SceneType() {
        // Delete all dynamically allocated shapes
        for (auto shape : shapes) {
            delete shape;
        }

        // Delete all dynamically allocated lights
        for (auto light : lights) {
            delete light;
        }

        // Delete all dynamically allocated materials
        for (auto material : materials) {
            delete material;
        }

        // Delete all dynamically allocated textures
        for (auto texture : textures) {
            delete texture;
        }

        // Delete all dynamically allocated vertices
        for (auto vertex : vertices) {
            delete vertex;
        }

        // Delete all dynamically allocated normal vectors
        for (auto normal : normalVectors) {
            delete normal;
        }

        // Delete all dynamically allocated texture coordinates
        for (auto texCoord : textureCoordinates) {
            delete texCoord;
        }

        // Delete the KDTree root node
        delete root;
    }

    // Getters
    const ColorType getBackgroundColor() const { return backgroundColor; }
    const std::vector<LightType*> getLights() const { return lights; }
    const std::vector<ShapeType*> getShapes() const { return shapes; }
    const KDTreeNode* getKDRoot() const { return root; }
    const std::vector<MaterialType*>& getMaterials() const { return materials; }
    const std::vector<TextureType*>& getTextures() const { return textures; }
    const std::vector<Vec3*>& getVertices() const { return vertices; }
    const std::vector<Vec3*>& getNormalVectors() const { return normalVectors; }
    const std::vector<Vec2*>& getTextureCoordinates() const { return textureCoordinates; }

    // Setters
    void setBackgroundColor(ColorType backgroundColor) { this->backgroundColor = backgroundColor; }
    void setShapes(std::vector<ShapeType*> shapes) { this->shapes = shapes; }
    void setKDRoot(KDTreeNode* kdRoot) { this->root = kdRoot; }

    // Add functions
    void addShape(ShapeType* shape) { this->shapes.push_back(shape); }
    void addLight(LightType* light) { this->lights.push_back(light); }
    void addMaterial(MaterialType* material) { this->materials.push_back(material); }
    void addTexture(TextureType* texture) { this->textures.push_back(texture); }
    void addVertex(Vec3* vertex) { this->vertices.push_back(vertex); }
    void addNormalVector(Vec3* normal) { this->normalVectors.push_back(normal); }
    void addTextureCoordinate(Vec2* texCoord) { this->textureCoordinates.push_back(texCoord); }
};
