#include "texture.h"
#include "color.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Texture constructor
Texture::Texture(const std::string& filename) {
    // Open the file
    std::ifstream textureFile(filename, std::ios::binary);
    if (!textureFile.is_open()) {
        throw std::runtime_error("Error: Unable to open texture file: " + filename);
    }

    // Read the PPM header
    std::string format;
    if (!(textureFile >> format >> this->width >> this->height >> this->maxValue)) {
        throw std::runtime_error("Error: Failed to read texture PPM header information.");
    }

    if (format != "P3") {
        throw std::runtime_error("Error: Unsupported PPM format. Only P3 format is supported.");
    }

    if (this->width <= 0 || this->height <= 0) {
        throw std::runtime_error("Error: Texture dimensions must be greater than zero.");
    }

    // Initialize the 2D array
    textureArray = new Color * [this->width];
    for (int i = 0; i < width; ++i) {
        textureArray[i] = new Color[this->height];
    }

    // Read pixel data
    for (int j = 0; j < this->height; ++j) {
        for (int i = 0; i < this->width; ++i) {
            double r, g, b;
            if (!(textureFile >> r >> g >> b)) {
                throw std::runtime_error("Error: Failed to read pixel data from texture file.");
            }

            // Map the RGB values to [0, 1]
            Color color(r / maxValue, g / maxValue, b / maxValue);

            this->textureArray[i][j] = color;
        }
    }
}

// Texture getters
const Color& Texture::getPixel(Vec2 textureCoordinate) const {
    if (textureCoordinate.x < 0.0 || textureCoordinate.x > 1.0 || textureCoordinate.y < 0.0 || textureCoordinate.y > 1.0) {
        throw std::out_of_range("Error: Pixel coordinates are out of bounds for texture");
    }
    int x = static_cast<int>(textureCoordinate.x * (this->width - 1));
    int y = static_cast<int>((1.0 - textureCoordinate.y) * (this->height - 1)); // Flip y for bottom-left origin
    return this->textureArray[x][y];
}

// Texture setters
void Texture::setPixel(const Vec2 textureCoordinate, const Color color) {
    if (textureCoordinate.x < 0.0 || textureCoordinate.x > 1.0 || textureCoordinate.y < 0.0 || textureCoordinate.y > 1.0) {
        throw std::out_of_range("Error: Pixel coordinates are out of bounds for texture");
    }
    int x = static_cast<int>(textureCoordinate.x * (this->width - 1));
    int y = static_cast<int>((1.0 - textureCoordinate.y) * (this->height - 1)); // Flip y for bottom-left origin
    this->textureArray[x][y] = color;
}

// NormalMap constructor
NormalMap::NormalMap(const std::string& filename) {
    // Open the file
    std::ifstream textureFile(filename, std::ios::binary);
    if (!textureFile.is_open()) {
        throw std::runtime_error("Error: Unable to open normal map file: " + filename);
    }

    // Read the PPM header
    std::string format;
    if (!(textureFile >> format >> this->width >> this->height >> this->maxValue)) {
        throw std::runtime_error("Error: Failed to read normal map PPM header information.");
    }

    if (format != "P3") {
        throw std::runtime_error("Error: Unsupported PPM format. Only P3 format is supported.");
    }

    if (this->width <= 0 || this->height <= 0) {
        throw std::runtime_error("Error: Normal map dimensions must be greater than zero.");
    }

    // Initialize the 2D array
    normalMapArray = new Vec3 * [this->width];
    for (int i = 0; i < this->width; ++i) {
        normalMapArray[i] = new Vec3[this->height];
    }

    // Read pixel data
    for (int j = 0; j < this->height; ++j) {
        for (int i = 0; i < this->width; ++i) {
            double r, g, b;
            if (!(textureFile >> r >> g >> b)) {
                throw std::runtime_error("Error: Failed to read pixel data from normal map file.");
            }

            // Map the RGB values from [0, maxValue] to [-1, 1]
            Vec3 normalVector((r / maxValue) * 2.0 - 1.0, (g / maxValue) * 2.0 - 1.0, (b / maxValue) * 2.0 - 1.0);

            // Normalize the normal vector
            normalVector = normalVector.normal();

            this->normalMapArray[i][j] = normalVector;
        }
    }
}

// NormalMap getters
const Vec3& NormalMap::getNormal(Vec2 textureCoordinate) const {
    if (textureCoordinate.x < 0.0 || textureCoordinate.x > 1.0 || textureCoordinate.y < 0.0 || textureCoordinate.y > 1.0) {
        throw std::out_of_range("Error: Pixel coordinates are out of bounds for texture");
    }
    int x = static_cast<int>(textureCoordinate.x * (this->width - 1));
    int y = static_cast<int>((1.0 - textureCoordinate.y) * (this->height - 1)); // Flip y for bottom-left origin
    return this->normalMapArray[x][y];;
}

// NormalMap setters
void NormalMap::setNormal(Vec2 textureCoordinate, const Vec3& normal) {
    if (textureCoordinate.x < 0.0 || textureCoordinate.x > 1.0 || textureCoordinate.y < 0.0 || textureCoordinate.y > 1.0) {
        throw std::out_of_range("Error: Pixel coordinates are out of bounds for texture");
    }
    int x = static_cast<int>(textureCoordinate.x * (this->width - 1));
    int y = static_cast<int>((1.0 - textureCoordinate.y) * (this->height - 1)); // Flip y for bottom-left origin
    this->normalMapArray[x][y] = normal.normal();
}