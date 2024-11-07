#pragma once

#include <fstream>
#include "vec.h"
#include "color.h"
#include "ImageSize.h"

// Abstract base class for textures
class ITexture {
public:
    virtual ~ITexture() = default;
    virtual const ImageSize& getImageSize() const = 0;
    virtual int getMaxValue() const = 0;
};

// Concrete class for Color textures
class Texture : public ITexture {
    ImageSize imageSize;
    int maxValue;
    Color** textureArray;

public:
    // Constructor
    Texture(const std::string& filename);

    // Destructor
    ~Texture() {
        for (int i = 0; i < this->imageSize.getWidth(); i++) {
            delete[] this->textureArray[i];
        }
        delete[] this->textureArray;
    }

    // Getters
    const Color& getPixel(int x, int y) const { return this->textureArray[x][y]; }

    // Setters
    void setPixel(int x, int y, const Color& color) { this->textureArray[x][y] = color; }

    // Override methods
    const ImageSize& getImageSize() const override { return this->imageSize; }
    int getMaxValue() const override { return this->maxValue; }
};

// Concrete class for Normal maps
class NormalMap : public ITexture {
};
