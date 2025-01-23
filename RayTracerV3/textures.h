#pragma once
#include <fstream>
#include "vec.h"
#include "color.h"

// Base class for ppm based textures
class AbstractTexture {
protected:
    int height = 0; // cant be changed
    int width = 0; // cant be changed
    int maxValue = 0; // cant be changed

public:
    // Virtual destructor
    virtual ~AbstractTexture() = default;

    // Getters
    int getMaxValue() const { return this->maxValue; }
    int getWidth() { return this->width; }
    int getHeight() { return this->height; }
};


class Texture : public AbstractTexture {
private:
    Color** textureArray; // cant be changed

public:
    // Constructor
    Texture(const std::string& filename);

    // Destructor
    ~Texture() {
        for (int i = 0; i < this->width; i++) {
            delete[] this->textureArray[i];
        }
        delete[] this->textureArray;
    }

    // Getters
    const Color& getPixel(const Vec2 textureCoordinate) const;

    // Setters
    void setPixel(const Vec2 textureCoordinate, const Color color);
};


class NormalMap : public AbstractTexture {
private:
    Vec3** normalMapArray;

public:
    // Constructor
    NormalMap(const std::string& filename);

    // Destructor
    ~NormalMap() {
        for (int i = 0; i < this->width; i++) {
            delete[] this->normalMapArray[i];
        }
        delete[] this->normalMapArray;
    }

    // Getters
    const Vec3& getNormal(Vec2 textureCoordinate) const;

    // Settrers
    void setNormal(Vec2 textureCoordinate, const Vec3& normal);
};
