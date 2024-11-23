#include "texture.h"
#include "color.h"

Texture::Texture(const std::string& filename) {
    // Open filename
    std::ifstream textureFile(filename, std::ios::binary);
    if (!textureFile.is_open()) {
        throw std::runtime_error("Error: Unable to open texture file: " + filename);
    }
    std::string format;
    int width, height, maxValue;

    // Read the PPM header
    if (!(textureFile >> format >> width >> height >> maxValue)) {
        throw std::runtime_error("Error: Failed to read texture PPM header information");
    }
    if (format != "P3") {
        throw std::runtime_error("Error: Unsupported PPM format. Only P3 format is supported.");
    }
    if (width <= 0 || height <= 0) {
        throw std::runtime_error("Error: Texture dimensions must be greater than zero.");
    }

    // Initialize member variables
    this->imageSize = ImageSize(width, height);
    this->maxValue = maxValue;
    this->textureArray = new Color * [width];
    for (int i = 0; i < width; ++i) {
        this->textureArray[i] = new Color[height];
    }

    // Read the pixel data
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            double r, g, b;
            if (!(textureFile >> r >> g >> b)) {
                throw std::runtime_error("Error: Failed to read pixel data from texture file.");
            }
            Color color(r / maxValue, g / maxValue, b / maxValue);
            this->setPixel(i, j, color);
        }
    }
}

const Color& Texture::getPixel(int u, int v) const {
    if (u < 0 || u >= this->imageSize.getWidth() || v < 0 || v >= this->imageSize.getHeight()) {
        throw std::out_of_range("Error: Pixel coordinates are out of bounds for texture");
    }
    return this->textureArray[u][v];
}

// Set pixel color with bounds checking
void Texture::setPixel(int u, int v, const Color& color) {
    if (u < 0 || u >= this->imageSize.getWidth() || v < 0 || v >= this->imageSize.getHeight()) {
        throw std::out_of_range("Error: Pixel coordinates are out of bounds for texture");
    }
    this->textureArray[u][v] = color;
}


NormalMap::NormalMap(const std::string& filename) {
    // Open filename
    std::ifstream textureFile(filename, std::ios::binary);
    if (!textureFile.is_open()) {
        throw std::runtime_error("Error: Unable to open texture file: " + filename);
    }
    std::string format;
    int width, height, maxValue;

    // Read the PPM header
    if (!(textureFile >> format >> width >> height >> maxValue)) {
        throw std::runtime_error("Error: Failed to read texture PPM header information");
    }
    if (format != "P3") {
        throw std::runtime_error("Error: Unsupported PPM format. Only P3 format is supported.");
    }
    if (width <= 0 || height <= 0) {
        throw std::runtime_error("Error: Texture dimensions must be greater than zero.");
    }

    // Initialize member variables
    this->imageSize = ImageSize(width, height);
    this->maxValue = maxValue;
    this->normalMapArray = new Vec3 * [width];
    for (int i = 0; i < width; ++i) {
        this->normalMapArray[i] = new Vec3[height];
    }

    // Read the pixel data
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            double x, y, z;
            if (!(textureFile >> x >> y >> z)) {
                throw std::runtime_error("Error: Failed to read normal data from normalMap file.");
            }
            Vec3 normal(x, y, z);
            this->setNormal(i, j, normal);
        }
    }
}


const Vec3& NormalMap::getNormal(int u, int v) const {
    if (u < 0 || u >= this->imageSize.getWidth() || v < 0 || v >= this->imageSize.getHeight()) {
        throw std::out_of_range("Error: Normal map coordinates are out of bounds for normal map");
    }
    return this->normalMapArray[u][v];
}

// Set normal vector with bounds checking
void NormalMap::setNormal(int u, int v, const Vec3& normal) {
    if (u < 0 || u >= this->imageSize.getWidth() || v < 0 || v >= this->imageSize.getHeight()) {
        throw std::out_of_range("Error: Normal map coordinate are out of bounds for normal map");
    }
    this->normalMapArray[u][v] = normal.normal(); // Ensure the normal vector is normalized
}