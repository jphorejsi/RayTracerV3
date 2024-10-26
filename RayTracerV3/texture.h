#pragma once
#include <vector>
#include "vec.h"
#include "color.h"
#include "ImageSize.h"

class TextureType {
    ImageSizeType imageSize;
    int maxValue;
    ColorType** textureArray;

public:
    // Constructor
    TextureType(int width, int height, int maxVal) : imageSize(width, height), maxValue(maxVal) {
        this->textureArray = new ColorType* [width];
        for (int i = 0; i < width; i++) {
            this->textureArray[i] = new ColorType[height];
        }
    }

    // Getters
    const ImageSizeType& getImageSize() const { return imageSize; }
    int getMaxValue() const { return maxValue; }
    const ColorType& getPixel(int x, int y) const { return textureArray[x][y]; }

    // Setters
    void setPixel(int x, int y, const ColorType& color) { textureArray[x][y] = color; }
};