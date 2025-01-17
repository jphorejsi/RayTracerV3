#include "materials.h"
#include "scene.h"
#include "shapes.h"
#include <stdexcept>
#include "renderer.h"

// BlinnPhong material constructor
BlinnPhong::BlinnPhong(Color od, Color os, double ka, double kd, double ks, double n) {
    if (od.getR() < 0.0 || od.getR() > 1.0 || od.getG() < 0.0 || od.getG() > 1.0 || od.getB() < 0.0 || od.getB() > 1.0) {
        throw std::invalid_argument("Diffuse color (od) RGB values must be between 0 and 1");
    }
    if (os.getR() < 0.0 || os.getR() > 1.0 || os.getG() < 0.0 || os.getG() > 1.0 || os.getB() < 0.0 || os.getB() > 1.0) {
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

// Getters for BlinnPhong
Color BlinnPhong::getOd(Vec2 textureCoordinate) const {
    if (this->getTexture()) {
        return this->texture->getPixel(textureCoordinate);
    }
    else return od;
}

// Setters for BlinnPhong
void BlinnPhong::setKa(double ka) {
    if (ka < 0.0 || ka > 1.0) {
        throw std::invalid_argument("Ambient coefficient (ka) must be between 0 and 1");
    }
    this->ka = ka;
}


void BlinnPhong::setKd(double kd) {
    if (kd < 0.0 || kd > 1.0) {
        throw std::invalid_argument("Diffuse coefficient (kd) must be between 0 and 1");
    }
    this->kd = kd;
}


void BlinnPhong::setKs(double ks) {
    if (ks < 0.0 || ks > 1.0) {
        throw std::invalid_argument("Specular coefficient (ks) must be between 0 and 1");
    }
    this->ks = ks;
}


void BlinnPhong::setN(double n) {
    if (n <= 0.0) {
        throw std::invalid_argument("Shininess exponent (n) must be greater than 0");
    }
    this->n = n;
}

// Shading method for BlinnPhong
Color BlinnPhong::shade(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const Shape& shape, int maxDepth) const {
    // Initialize diffuse and specular
    Color diffuse(0, 0, 0);
    Color specular(0, 0, 0);

    // Find texture coordinates
    Vec2 tc = shape.getTextureCoordinate(intersectionPoint);

    // Calculate normal
    Vec3 normal = shape.getNormal(intersectionPoint, tc);

    // Ambient component
    Color ambient = this->getOd(tc) * this->getKa();

    // Calculate diffuse and specular components for each light
    for (AbstractLight* light : scene.getLights()) {
        // Determine shadow factor
        double sF = light->calculateShadowFactor(intersectionPoint, scene);

        // Diffuse component
        diffuse = diffuse + light->calculateDiffuse(*this, intersectionPoint, normal, tc) * sF;

        // Specular component
        specular = specular + light->calculateSpecular(*this, intersectionPoint, normal, ray.getDirection()) * sF;
    }

    // Implement depth cueing if it exists in the scene
    if (scene.getDepthCue()) {
        double dcf = scene.getDepthCue()->calculateDepthCueingFactor((ray.origin - intersectionPoint).length());
        Color depthCueContribution = scene.getDepthCue()->getDepthCueColor() * (1 - dcf);

        // Modify the components directly
        ambient = (ambient * dcf) + depthCueContribution;
        diffuse = diffuse * dcf;
        specular = specular * dcf;
    }

    // Combine ambient, diffuse, and specular components
    return ambient + diffuse + specular;
}

// Constructor for reflective blinn phong material
ReflectiveBlinnPhong::ReflectiveBlinnPhong(Color od, Color os, double ka, double kd, double ks, double n, double alpha, double eta) : BlinnPhong(od, os, ka, kd, ks, n) {
    if (alpha < 0.0 || alpha > 1.0) {
        throw std::invalid_argument("Alpha (reflectivity) must be between 0 and 1");
    }
    if (eta <= 0.0) {
        throw std::invalid_argument("Eta (index of refraction) must be greater than 0");
    }
    this->alpha = alpha;
    this->eta = eta;
}

// Setters for ReflectiveBlinnPhong
void ReflectiveBlinnPhong::setAlpha(double alpha) {
    if (alpha < 0.0 || alpha > 1.0) {
        throw std::invalid_argument("Alpha (opacity) must be between 0 and 1");
    }
    this->alpha = alpha;
}

void ReflectiveBlinnPhong::setEta(double eta) {
    if (eta < 0.0) {
        throw std::invalid_argument("Eta (refractive index) must be greater than 0");
    }
    this->eta = eta;
}

// Shading method for ReflectiveBlinnPhong
Color ReflectiveBlinnPhong::shade(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const Shape& shape, int maxDepth) const {
    // Initialize diffuse and specular
    Color diffuse(0, 0, 0);
    Color specular(0, 0, 0);

    // Find texture coordinates
    Vec2 tc = shape.getTextureCoordinate(intersectionPoint);

    // Calculate normal and check if entering or exiting
    Vec3 normal = shape.getNormal(intersectionPoint, tc);
    Vec3 incident = ray.getDirection().normal() * -1;
    bool entering = dot(incident, normal) > 0;
    if (!entering) normal = -normal;

    Color ambient = this->getOd(tc) * this->getKa();

    // Default refractive indices
    double etaI = 1.0;  // Assume air for the outside medium
    double etaT = entering ? this->getEta() : 1.0;
    double etaRatio = etaI / etaT;

    // Compute Fresnel reflectance (Schlick's approximation)
    double cosThetaI = dot(incident, normal);
    double F0 = pow((etaT - etaI) / (etaT + etaI), 2);
    double Fr = F0 + (1 - F0) * pow(1 - cosThetaI, 5);

    for (const AbstractLight* light : scene.getLights()) {
        // Determine shadow factor
        double sF = light->calculateShadowFactor(intersectionPoint, scene);

        // Diffuse component
        diffuse = diffuse + light->calculateDiffuse(*this, intersectionPoint, normal, tc) * sF;

        // Specular component
        specular = specular + light->calculateSpecular(*this, intersectionPoint, normal, ray.getDirection()) * sF;
    }

    // Reflection
    Color reflection(0, 0, 0);
    if (this->getKs() > 0.0) {
        Vec3 reflectionDirection = normal * 2 * cosThetaI - incident;
        Ray reflectionRay(intersectionPoint, reflectionDirection);
        reflection = Renderer::traceRay(reflectionRay, scene, maxDepth - 1) * Fr;
    }

    // Refraction
    Color refraction(0, 0, 0);
    if (this->getAlpha() < 1.0) {
        double discriminant = 1 - etaRatio * etaRatio * (1 - cosThetaI * cosThetaI);
        if (discriminant > 0) {
            Vec3 refractionDirection = ((normal * -sqrt(discriminant)) + ((normal * cosThetaI) - incident) * etaRatio).normal();
            Ray refractionRay(intersectionPoint, refractionDirection);
            refraction = Renderer::traceRay(refractionRay, scene, maxDepth - 1) * (1 - Fr) * (1 - this->getAlpha());
        }
    }

    // Combine illumination, reflection, and refraction
    return ambient + diffuse + specular + reflection + refraction;
}

