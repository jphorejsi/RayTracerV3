#pragma once

#include <fstream>
#include "vec.h"
#include "color.h"
#include "ImageSize.h"

class AbstractTexture {
protected:
    ImageSize imageSize;
    int maxValue;

public:
    // Virtual destructor
    virtual ~AbstractTexture() = default;

    // Getters
    int getMaxValue() const { return this->maxValue; }
    virtual const ImageSize& getImageSize() const { return imageSize; }
};


class Texture : public AbstractTexture {
private:
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
    const Color& getPixel(double u, double v) const;

    // Setters
    void setPixel(double u, double v, const Color& color);
};

class NormalMap : public AbstractTexture {
private:
    Vec3** normalMapArray;

public:
    // Constructor
    NormalMap(const std::string& filename);

    // Destructor
    ~NormalMap() {
        for (int i = 0; i < this->imageSize.getWidth(); i++) {
            delete[] this->normalMapArray[i];
        }
        delete[] this->normalMapArray;
    }

    // Overridden methods
    const Vec3& getNormal(double u, double v) const;

    void setNormal(double u, double v, const Vec3& normal);
};
