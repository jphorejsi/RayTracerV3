#pragma once
#pragma once

#include <vector>
#include "color.h"
#include "shapes.h"
#include "lights.h"
#include "kdtree.h"

class Scene {
private:
    Color backgroundColor;
    std::vector<IShape*> shapes;  // Shapes are stored here directly
    std::vector<ILight*> lights;
    KDTreeNode* root;

public:
    // Constructors
    Scene() = default;
    Scene(Color backgroundColor, const std::vector<IShape*>& shapes, const std::vector<ILight*>& lights, KDTreeNode* root) : backgroundColor(backgroundColor), shapes(shapes), lights(lights), root(root) {}

    // Destructor
    ~Scene() {
        for (auto shape : shapes) delete shape;
        for (auto light : lights) delete light;
        delete root;
    }

    // Getters with const correctness
    const Color& getBackgroundColor() const { return backgroundColor; }
    const std::vector<ILight*>& getLights() const { return lights; }
    const std::vector<IShape*>& getShapes() const { return shapes; }
    const KDTreeNode* getKDRoot() const { return root; }
};


class SceneBuilder {
private:
    Color backgroundColor;
    std::vector<IShape*> shapes;
    std::vector<ILight*> lights;
    KDTreeNode* root = nullptr;

public:
    // Getters
    const Color& getBackgroundColor() const { return backgroundColor; }
    const std::vector<ILight*>& getLights() const { return lights; }
    const std::vector<IShape*>& getShapes() const { return shapes; }
    const KDTreeNode* getKDRoot() const { return root; }

    // Setters
    SceneBuilder& setBackgroundColor(const Color& color) { this->backgroundColor = color; return *this; }
    SceneBuilder& addShape(IShape* shape) { this->shapes.push_back(shape); return *this; }
    SceneBuilder& addLight(ILight* light) { this->lights.push_back(light); return *this; }
    SceneBuilder& setKDRoot(KDTreeNode* root) { this->root = root; return *this; }

    // Build function
    Scene build() const {
        return Scene(backgroundColor, shapes, lights, root);
    }
};