#include "materials.h"

#include "scene.h"
#include "shapes.h"

// Constructor
RGBMaterial::RGBMaterial(Color color) {
    if (color.getR() < 0 || color.getR() > 255 ||
        color.getG() < 0 || color.getG() > 255 ||
        color.getB() < 0 || color.getB() > 255) {
        throw std::invalid_argument("RGB values must be between 0 and 255");
    }
    this->color = color;
}

// Setters
void RGBMaterial::setColor(Color color) {
    if (color.getR() < 0 || color.getR() > 255 ||
        color.getG() < 0 || color.getG() > 255 ||
        color.getB() < 0 || color.getB() > 255) {
        throw std::invalid_argument("RGB values must be between 0 and 255");
    }
    this->color = color;
}

// Shading
Color RGBMaterial::shadeRay(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const AbstractShape* shape) const {
    return color;
}

// Constructor
PhongMaterial::PhongMaterial(Color od, Color os, double ka, double kd, double ks, double n) {
    if (od.getR() < 0.0 || od.getR() > 1.0 ||
        od.getG() < 0.0 || od.getG() > 1.0 ||
        od.getB() < 0.0 || od.getB() > 1.0) {
        throw std::invalid_argument("Diffuse color (od) RGB values must be between 0 and 1");
    }
    if (os.getR() < 0.0 || os.getR() > 1.0 ||
        os.getG() < 0.0 || os.getG() > 1.0 ||
        os.getB() < 0.0 || os.getB() > 1.0) {
        throw std::invalid_argument("Specular color (os) RGB values must be between 0 and 1");
    }
    if (ka < 0.0 || ka > 1.0) {
        throw std::invalid_argument("Ambient coefficient (ka) must be between 0 and 1");
    }
    if (kd < 0.0 || kd > 1.0) {
        throw std::invalid_argument("Diffuse coefficient (kd) must be between 0 and 1");
    }
    if (ks < 0.0 || ks > 1.0) {
        throw std::invalid_argument("Specular coefficient (ks) must be between 0 and 1");
    }
    if (n <= 0.0) {
        throw std::invalid_argument("Shininess exponent (n) must be greater than 0");
    }
    this->od = od;
    this->os = os;
    this->ka = ka;
    this->kd = kd;
    this->ks = ks;
    this->n = n;
}

// Setters
void PhongMaterial::setOs(Color os) {
    if (os.getR() < 0.0 || os.getR() > 1.0 ||
        os.getG() < 0.0 || os.getG() > 1.0 ||
        os.getB() < 0.0 || os.getB() > 1.0) {
        throw std::invalid_argument("RGB values must be between 0 and 255");
    }
    this->os = os;
}

void PhongMaterial::setKa(double ka) {
    if (ka < 0.0 || ka > 1.0) {
        throw std::invalid_argument("Ambient coefficient (ka) must be between 0 and 1");
    }
    this->ka = ka;
}

void PhongMaterial::setKd(double kd) {
    if (kd < 0.0 || kd > 1.0) {
        throw std::invalid_argument("Diffuse coefficient (kd) must be between 0 and 1");
    }
    this->kd = kd;
}

void PhongMaterial::setKs(double ks) {
    if (ks < 0.0 || ks > 1.0) {
        throw std::invalid_argument("Specular coefficient (ks) must be between 0 and 1");
    }
    this->ks = ks;
}

void PhongMaterial::setN(double n) {
    if (n <= 0.0) {
        throw std::invalid_argument("Shininess exponent (n) must be greater than 0");
    }
    this->n = n;
}

// Shading
Color PhongMaterial::shadeRay(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const AbstractShape* shape) const { // pass its own shape?
    Color ambient = this->od * this->ka;
    Color diffuse(0, 0, 0);
    Color specular(0, 0, 0);
    Vec3 N = shape->getNormal(intersectionPoint);

    // Calculate diffuse and specular components
    for (AbstractLight* light : scene.getLights()) {
        if (light->illuminates(intersectionPoint, scene)) {
            diffuse += light->calculateDiffuse(this, intersectionPoint, N);
            specular += light->calculateSpecular(this, intersectionPoint, N, ray.getDirection());
        }
    }

    // Check if depth cueing is enabled
    if (scene.getDepthCue()) {
        Color finalColor = ambient + diffuse + specular;
        double depthCueingFactor = scene.getDepthCue()->calculateDepthCueingFactor((ray.getOrigin() - intersectionPoint).length());

        // Blend all components with the depth cue color
        finalColor = finalColor * depthCueingFactor + scene.getDepthCue()->getDepthCue() * (1.0 - depthCueingFactor);
        return finalColor;
    }

    return ambient + diffuse + specular;
}

// Constructor
ReflectivePhongMaterial::ReflectivePhongMaterial(Color od, Color os, double ka, double kd, double ks, double n, double alpha, double eta) : PhongMaterial(od, os, ka, kd, ks, n) {
    if (alpha < 0.0 || alpha > 1.0) {
        throw std::invalid_argument("Alpha (reflectivity) must be between 0 and 1");
    }
    if (eta <= 0.0) {
        throw std::invalid_argument("Eta (refractive index) must be greater than 0");
    }
    this->alpha = alpha;
    this->eta = eta;
}

// Setters
void ReflectivePhongMaterial::setAlpha(double alpha) {
    if (alpha < 0.0 || alpha > 1.0) {
        throw std::invalid_argument("Alpha (reflectivity) must be between 0 and 1");
    }
    this->alpha = alpha;
}

void ReflectivePhongMaterial::setEta(double eta) {
    if (eta <= 0.0) {
        throw std::invalid_argument("Eta (refractive index) must be greater than 0");
    }
    this->eta = eta;
}

Color ReflectivePhongMaterial::shadeRay(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const AbstractShape* shape) const { // Finish
    return Color(1, 1, 1);
}