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
    std::vector<Shape*> shapes;
    std::vector<AbstractLight*> lights;
    BVHNode* BVHRoot = nullptr;
    DepthCue* depthCue = nullptr;

public:
    // Constructors
    Scene(const Color& backgroundColor) : backgroundColor(backgroundColor) {};

    // Destructor
    ~Scene() {
        for (auto shape : shapes) delete shape;
        for (auto light : lights) delete light;
        delete BVHRoot;
        delete depthCue;
    }

    // Getters
    const Color getBackgroundColor() const { return this->backgroundColor; }
    const std::vector<Shape*>& getShapes() const { return this->shapes; }
    const std::vector<AbstractLight*>& getLights() const { return this->lights; }
    const BVHNode* getBVHRoot() const { return this->BVHRoot; }
    const DepthCue* getDepthCue() const { return this->depthCue; }

    // Setters
    void setDepthCue(DepthCue* depthCue) { this->depthCue = depthCue; }

    // Other methods
    void addShape(Shape* shape) {
        if (shape) { shapes.push_back(shape); }
        else { throw std::invalid_argument("Cannot add a null shape to the scene."); }
    }
    
    void addMesh(const Mesh& mesh) {
        for (const auto& face : mesh.getFaces()) {
            shapes.push_back(new Triangle(face)); // Directly add Triangle (copy by value)
        }
    }

    void addLight(AbstractLight* light) {
        if (light) { lights.push_back(light); }
        else { throw std::invalid_argument("Cannot add a null light to the scene."); }
    }

    void buildBVH(int maxShapesPerLeaf = 2) {
        // Clear any existing BVH
        if (BVHRoot) {
            delete BVHRoot;
            BVHRoot = nullptr;
        }
        // Create new BVH root
        BVHRoot = new BVHNode();
        // Build from our shapes
        BVHRoot->buildBVH(this->shapes, maxShapesPerLeaf);
    }
};