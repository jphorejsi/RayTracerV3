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
            float r, g, b;
            if (!(textureFile >> r >> g >> b)) {
                throw std::runtime_error("Error: Failed to read pixel data from texture file.");
            }
            Color color(r / maxValue, g / maxValue, b / maxValue);
            this->setPixel(i, j, color);
        }
    }
}