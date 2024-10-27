#pragma once
#include <vector>
#include "shapes.h"
#include "lights.h"
#include "material.h"

// ShapeContainer Interface
class IShapeContainer {
public:
    virtual void addShape(ShapeType* shape) = 0;
    virtual std::vector<ShapeType*> getShapes() const = 0;
};

// LightContainer Interface
class ILightContainer {
public:
    virtual void addLight(LightType* light) = 0;
    virtual std::vector<LightType*> getLights() const = 0;
};

// MaterialContainer Interface
class IMaterialContainer {
public:
    virtual void addMaterial(MaterialType* material) = 0;
    virtual std::vector<MaterialType*> getMaterials() const = 0;
};

// Implementation
class ShapeContainer : public IShapeContainer {
private:
    std::vector<ShapeType*> shapes;

public:
    void addShape(ShapeType* shape) override {
        shapes.push_back(shape);
    }

    std::vector<ShapeType*> getShapes() const override {
        return shapes;
    }
};

class LightContainer : public ILightContainer {
private:
    std::vector<LightType*> lights;

public:
    void addLight(LightType* light) override {
        lights.push_back(light);
    }

    std::vector<LightType*> getLights() const override {
        return lights;
    }
};

class MaterialContainer : public IMaterialContainer {
private:
    std::vector<MaterialType*> materials;

public:
    void addMaterial(MaterialType* material) override {
        materials.push_back(material);
    }

    std::vector<MaterialType*> getMaterials() const override {
        return materials;
    }
};
