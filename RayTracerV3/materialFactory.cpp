#include "materialFactory.h"
#include <stdexcept>

// Factory method to create an RGBMaterial with a specified color
IMaterial* MaterialFactory::createRGBMaterial(const Color& color) {
    // Check if color values are within the range [0, 1]
    if (color.getR() < 0 || color.getR() > 1 || color.getG() < 0 || color.getG() > 1 || color.getB() < 0 || color.getB() > 1) {
        throw std::invalid_argument("Error: RGB color values must be between 0 and 1.");
    }
    return new RGBMaterial(color);
}

// Factory method to create a PhongMaterial with specified properties
IMaterial* MaterialFactory::createPhongMaterial(const Color& od, const Color& os, float ka, float kd, float ks, float n) {
    // Check if color values are within the range [0, 1]
    if (od.getR() < 0 || od.getR() > 1 || od.getG() < 0 || od.getG() > 1 || od.getB() < 0 || od.getB() > 1) {
        throw std::invalid_argument("Error: Diffuse color (od) values must be between 0 and 1.");
    }
    if (os.getR() < 0 || os.getR() > 1 || os.getG() < 0 || os.getG() > 1 || os.getB() < 0 || os.getB() > 1) {
        throw std::invalid_argument("Error: Specular color (os) values must be between 0 and 1.");
    }
    // Check if coefficients and exponent are non-negative
    if (ka < 0 || kd < 0 || ks < 0 || n < 0) {
        throw std::invalid_argument("Error: Coefficients (ka, kd, ks) and exponent (n) must be non-negative.");
    }

    return new PhongMaterial(od, os, ka, kd, ks, n);
}
