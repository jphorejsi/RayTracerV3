#include "texture.h"
#include "color.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

//Texture::Texture(const std::string& filename) {
//    // Open filename
//    std::ifstream textureFile(filename, std::ios::binary);
//    if (!textureFile.is_open()) {
//        throw std::runtime_error("Error: Unable to open texture file: " + filename);
//    }
//    std::string format;
//    int width, height, maxValue;
//
//    // Read the PPM header
//    if (!(textureFile >> format >> width >> height >> maxValue)) {
//        throw std::runtime_error("Error: Failed to read texture PPM header information");
//    }
//    if (format != "P3") {
//        throw std::runtime_error("Error: Unsupported PPM format. Only P3 format is supported.");
//    }
//    if (width <= 0 || height <= 0) {
//        throw std::runtime_error("Error: Texture dimensions must be greater than zero.");
//    }
//
//    // Initialize member variables
//    this->imageSize = ImageSize(width, height);
//    this->maxValue = maxValue;
//    this->textureArray = new Color * [width];
//    for (int i = 0; i < width; ++i) {
//        this->textureArray[i] = new Color[height];
//    }
//
//    // Read the pixel data
//    for (int j = 0; j < height; ++j) {
//        for (int i = 0; i < width; ++i) {
//            double r, g, b;
//            if (j == 753 && i == 5) {
//                if (!(textureFile >> r >> g >> b)) {
//                    throw std::runtime_error("Error: Failed to read pixel data from texture file.");
//                }
//                if (r == 0 && g == 0 && g == 0) {
//                    std::cout << "error at j" << j << " and i" << i << std::endl;
//                }
//                Color color(r / maxValue, g / maxValue, b / maxValue);
//                double u = static_cast<double>(i) / (width - 1);
//                double v = static_cast<double>(j) / (height - 1);
//                this->setPixel(u, v, color); // Use normalized coordinates
//            }
//        }
//    }
//}

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
            if (r == 0 && g == 0 && b == 0) {
                throw std::runtime_error("Error: Failed to read pixel data from texture file.");
            }
            Color color(r / maxValue, g / maxValue, b / maxValue);
            this->textureArray[i][j] = color;
        }
    }
}



const Color& Texture::getPixel(double u, double v) const {
    if (u < 0.0 || u > 1.0 || v < 0.0 || v >= 1.0) {
        throw std::out_of_range("Error: Pixel coordinates are out of bounds for texture");
    }
    int x = static_cast<int>(u * (this->imageSize.getWidth() - 1));
    int y = static_cast<int>(v * (this->imageSize.getHeight() - 1));
    Color color = this->textureArray[x][y];
    if (color.getR() == 0 && color.getG() == 0 && color.getB() == 0) {
        throw std::runtime_error("Error: Failed to read pixel data from texture file.");
    }
    return color;
}

//void Texture::setPixel(double u, double v, const Color& color) {
//    if (u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0) {
//        throw std::out_of_range("Error: Normalized coordinates are out of bounds for texture");
//    }
//    int x = static_cast<int>(u * (this->imageSize.getWidth() - 1));
//    int y = static_cast<int>(v * (this->imageSize.getHeight() - 1));
//    if (color.getR() == 0 && color.getG() == 0 && color.getB() == 0) {
//        throw std::runtime_error("Error: Failed to read pixel data from texture file.");
//    }
//    this->textureArray[x][y] = color;
//    if (this->textureArray[x][y].getR() == 0 && this->textureArray[x][y].getG() == 0 && this->textureArray[x][y].getB() == 0) {
//        throw std::runtime_error("Error: Failed to read pixel data from texture file.");
//    }
//}


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
                    throw std::runtime_error("Error: Failed to read pixel data from texture file.");
                }
                Vec3 normalVector(x / maxValue, y / maxValue, z / maxValue);
                double u = static_cast<double>(i) / (width - 1);
                double v = static_cast<double>(j) / (height - 1);
                this->setNormal(u, v, normalVector); // Use normalized coordinates
            }
        }
    }


const Vec3& NormalMap::getNormal(double u, double v) const {
    if (u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0) {
        throw std::out_of_range("Error: Normal map coordinates are out of bounds for normal map");
    }
    int x = static_cast<int>(u * (this->imageSize.getWidth() - 1));
    int y = static_cast<int>(v * (this->imageSize.getHeight() - 1));
    return this->normalMapArray[x][y];
}


void NormalMap::setNormal(double u, double v, const Vec3& normal) {
    if (u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0) {
        throw std::out_of_range("Error: Normalized coordinates are out of bounds for normal map");
    }
    int x = static_cast<int>(u * (this->imageSize.getWidth() - 1));
    int y = static_cast<int>(v * (this->imageSize.getHeight() - 1));
    this->normalMapArray[x][y] = normal.normal(); // Ensure the normal vector is normalized
}
