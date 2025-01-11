#include "materials.h"
#include "scene.h"
#include "shapes.h"
#include <stdexcept>
#include "renderer.h"

// Constructor definition (no default arguments here)
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


// Setters
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


Color BlinnPhong::shade(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const Shape& shape, int maxDepth) const {
    // Initialize diffuse and specular
    Color diffuse(0, 0, 0);
    Color specular(0, 0, 0);

    // Find texture coordinates
    Vec2 tc = shape.getTextureCoordinate(intersectionPoint);

    // Calculate normal
    Vec3 normal = shape.getNormal(intersectionPoint);

    // Ambient component
    Color ambient = this->getOd(tc) * this->getKa();

    // Calculate diffuse and specular components for each light
    for (AbstractLight* light : scene.getLights()) {
        // Determine shadow factor
        double sF = light->calculateShadowFactor(intersectionPoint, scene);

        // Diffuse component
        diffuse += light->calculateDiffuse(*this, intersectionPoint, normal, tc) * sF;

        // Specular component
        specular += light->calculateSpecular(*this, intersectionPoint, normal, ray.getDirection()) * sF;
    }
    
    if (scene.getDepthCue()) {
        double dcf = scene.getDepthCue()->calculateDepthCueingFactor((ray.origin - intersectionPoint).length());
        return (ambient + diffuse + specular)* dcf + scene.getDepthCue()->getDepthCueColor() * (1 - dcf);
    }

    // Combine ambient, diffuse, and specular components
    return ambient + diffuse + specular;

}




//void BlinnPhong::setAlpha(double alpha) {
//    if (alpha < 0.0 || alpha > 1.0) {
//        throw std::invalid_argument("Alpha (opacity) must be between 0 and 1");
//    }
//    this->alpha = alpha;
//}
//
//void BlinnPhong::setEta(double eta) {
//    if (eta < 0.0) {
//        throw std::invalid_argument("Eta (refractive index) must be greater than 0");
//    }
//    this->eta = eta;
//}



Color ReflectiveBlinnPhong::shade(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const Shape& shape, int maxDepth) const {
    // Calculate normal and check if entering or exiting
    Vec3 normal = shape.getNormal(intersectionPoint);
    Vec3 incident = ray.getDirection().normal() * -1;
    bool entering = dot(incident, normal) > 0;
    if (!entering) normal = -normal;

    // Default refractive indices
    double etaI = 1.0;  // Assume air for the outside medium
    double etaT = entering ? this->getEta() : 1.0;
    double etaRatio = etaI / etaT;

    // Compute Fresnel reflectance (Schlick's approximation)
    double cosThetaI = dot(incident, normal);
    double F0 = pow((etaT - etaI) / (etaT + etaI), 2);
    double Fr = F0 + (1 - F0) * pow(1 - cosThetaI, 5);



    Color ambient = this->getOd(shape.getTextureCoordinate(intersectionPoint)) * this->getKa();
    Color diffuse(0, 0, 0);
    Color specular(0, 0, 0);

    for (const AbstractLight* light : scene.getLights()) {
        double shadowFactor = light->calculateShadowFactor(intersectionPoint, scene);
        diffuse += light->calculateDiffuse(*this, intersectionPoint, normal, shape.getTextureCoordinate(intersectionPoint)) * shadowFactor;
        specular += light->calculateSpecular(*this, intersectionPoint, normal, ray.getDirection()) * shadowFactor;
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

