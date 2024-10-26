#pragma once

#include <vector>
#include "color.h"
#include "shapes.h"
#include "lights.h"
//#include "kdtree.h"


class SceneType {
	ColorType backgroundColor;
	std::vector<ShapeType*> shapes;
	std::vector<LightType*> lights;
	//KDTreeNode* root;

public:
	// Constructor
	SceneType() = default;

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
	}

	// Getters
	const ColorType getBackgroundColor() const { return backgroundColor; }
	const std::vector<LightType*> getLights() const { return lights; }
	const std::vector<ShapeType*> getShapes() const { return shapes; }
	//const KDTreeNode* getKDRoot() const { return root; }

	// Setters
	void setBackgroundColor(ColorType backgroundColor) { this->backgroundColor = backgroundColor; }
	void setShapes(std::vector<ShapeType*> shapes) { this->shapes = shapes; }
	//void setKDRoot(KDTreeNode* kdRoot) { this->root = kdRoot; }  // Setter for KDTree root

	// Other methods
	void addShape(ShapeType* shape) { this->shapes.push_back(shape); }
	void addLight(LightType* light) { this->lights.push_back(light); }
};