//#pragma once
//
//#include <vector>
//#include "color.h"
//#include "shapes.h"
//#include "lights.h"
//#include "kdtree.h"
//
//
//class Scene {
//private:
//    Color backgroundColor;
//    std::vector<IShape*> shapes;
//    std::vector<ILight*> lights;
//    KDTreeNode* root;
//
//    std::vector<IMaterial*> materials;
//    std::vector<Texture*> textures;
//    std::vector<NormalMap*> normalMaps;
//    std::vector<Vec3*> vertices;
//    std::vector<Vec3*> vertexNormals;
//    std::vector<Vec2*> textureCoordinates;
//
//public:
//    // Constructors
//    Scene() = default;
//    Scene(Color backgroundColor, const std::vector<IShape*>& shapes, const std::vector<ILight*>& lights, KDTreeNode* root, const std::vector<IMaterial*>& materials, const std::vector<Texture*>& textures, const std::vector<NormalMap*>& normalMaps, const std::vector<Vec3*>& vertices, const std::vector<Vec3*>& vertexNormals, const std::vector<Vec2*>& textureCoordinates)
//        : backgroundColor(backgroundColor), shapes(shapes), lights(lights), root(root), materials(materials), textures(textures), normalMaps(normalMaps), vertices(vertices), vertexNormals(vertexNormals), textureCoordinates(textureCoordinates) {}
//
//    // Destructor
//    ~Scene() {
//        for (auto shape : shapes) delete shape;
//        for (auto light : lights) delete light;
//        for (auto material : materials) delete material;
//        for (auto texture : textures) delete texture;
//        for (auto vertex : vertices) delete vertex;
//        for (auto normal : vertexNormals) delete normal;
//        for (auto texCoord : textureCoordinates) delete texCoord;
//        delete root;
//    }
//
//    // Getters with const correctness and returning references for large data
//    const Color& getBackgroundColor() const { return backgroundColor; }
//    const std::vector<ILight*>& getLights() const { return lights; }
//    const std::vector<IShape*>& getShapes() const { return shapes; }
//    const KDTreeNode* getKDRoot() const { return root; }
//    const std::vector<IMaterial*>& getMaterials() const { return materials; }
//    const std::vector<Texture*>& getTextures() const { return textures; }
//    const std::vector<Vec3*>& getVertices() const { return vertices; }
//    const std::vector<Vec3*>& getVertexNormals() const { return vertexNormals; }
//    const std::vector<Vec2*>& getTextureCoordinates() const { return textureCoordinates; }
//    const std::vector<NormalMap*>& getNormalMaps() const { return normalMaps; }
//};
//
//
//class SceneBuilder {
//private:
//    Color backgroundColor;
//    std::vector<IShape*> shapes;
//    std::vector<ILight*> lights;
//    std::vector<IMaterial*> materials;
//    std::vector<Texture*> textures;
//    std::vector<NormalMap*> normalMaps;
//    std::vector<Vec3*> vertices;
//    std::vector<Vec3*> vertexNormals;
//    std::vector<Vec2*> textureCoordinates;
//    KDTreeNode* root = nullptr;
//
//public:
//    // Getters
//    const Color& getBackgroundColor() const { return backgroundColor; }
//    const std::vector<ILight*>& getLights() const { return lights; }
//    const std::vector<IShape*>& getShapes() const { return shapes; }
//    const KDTreeNode* getKDRoot() const { return root; }
//    const std::vector<IMaterial*>& getMaterials() const { return materials; }
//    const std::vector<Texture*>& getTextures() const { return textures; }
//    const std::vector<Vec3*>& getVertices() const { return vertices; }
//    const std::vector<Vec3*>& getVertexNormals() const { return vertexNormals; }
//    const std::vector<Vec2*>& getTextureCoordinates() const { return textureCoordinates; }
//    const std::vector<NormalMap*>& getNormalMaps() const { return normalMaps; }
//
//    // Setters
//    SceneBuilder& setBackgroundColor(const Color& color) { this->backgroundColor = color; return *this; }
//    SceneBuilder& addShape(IShape* shape) { this->shapes.push_back(shape); return *this; }
//    SceneBuilder& addLight(ILight* light) { this->lights.push_back(light); return *this; }
//    SceneBuilder& addMaterial(IMaterial* material) { this->materials.push_back(material); return *this; }
//    SceneBuilder& addTexture(Texture* texture) { this->textures.push_back(texture); return *this; }
//    SceneBuilder& addNormalMap(NormalMap* normalMap) { this->normalMaps.push_back(normalMap); return *this; }
//    SceneBuilder& addVertex(Vec3* vertex) { this->vertices.push_back(vertex); return *this; }
//    SceneBuilder& addVertexNormal(Vec3* normal) { this->vertexNormals.push_back(normal); return *this; }
//    SceneBuilder& addTextureCoordinate(Vec2* texCoord) { this->textureCoordinates.push_back(texCoord); return *this; }
//    SceneBuilder& setKDRoot(KDTreeNode* root) { this->root = root; return *this; }
//
//    // Build function
//    Scene build() const {
//        return Scene(backgroundColor, shapes, lights, root, materials, textures, normalMaps, vertices, vertexNormals, textureCoordinates);
//    }
//};
