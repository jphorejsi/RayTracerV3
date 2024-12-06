#include "materialFactory.h"
#include <stdexcept>

Material* MaterialFactory::createMaterial(const Color& od, const Color& os, double ka, double kd, double ks, double n) {
    if (od.getR() < 0 || od.getR() > 1 || od.getG() < 0 || od.getG() > 1 || od.getB() < 0 || od.getB() > 1) {
        throw std::invalid_argument("Error: Diffuse color (od) values must be between 0 and 1.");
    }
    if (os.getR() < 0 || os.getR() > 1 || os.getG() < 0 || os.getG() > 1 || os.getB() < 0 || os.getB() > 1) {
        throw std::invalid_argument("Error: Specular color (os) values must be between 0 and 1.");
    }
    if (ka < 0 || kd < 0 || ks < 0 || n < 0) {
        throw std::invalid_argument("Error: Coefficients (ka, kd, ks) and exponent (n) must be non-negative.");
    }
    return new Material(od, os, ka, kd, ks, n);
}


Material* MaterialFactory::createReflectiveMaterial(const Color& od, const Color& os, double ka, double kd, double ks, double n, double alpha, double eta) {
    if (od.getR() < 0 || od.getR() > 1 || od.getG() < 0 || od.getG() > 1 || od.getB() < 0 || od.getB() > 1) {
        throw std::invalid_argument("Error: Diffuse color (od) values must be between 0 and 1.");
    }
    if (os.getR() < 0 || os.getR() > 1 || os.getG() < 0 || os.getG() > 1 || os.getB() < 0 || os.getB() > 1) {
        throw std::invalid_argument("Error: Specular color (os) values must be between 0 and 1.");
    }
    if (ka < 0 || kd < 0 || ks < 0 || n < 0) {
        throw std::invalid_argument("Error: Coefficients (ka, kd, ks) and exponent (n) must be non-negative.");
    }
    if (alpha < 0 || alpha > 1) {
        throw std::invalid_argument("Error: Reflectivity (alpha) must be between 0 and 1.");
    }
    if (eta < 0) {
        throw std::invalid_argument("Error: Refractive index (eta) must be non-negative.");
    }
    return new Material(od, os, ka, kd, ks, n, alpha, eta);
}
