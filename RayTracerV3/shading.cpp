#include "Shading.h"
#include "Rendering.h"

Color PhongShading::shadeRay(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const AbstractShape& shape) {
    Material* material = shape.getMaterial();

    // Return a default color if the shape has no material
    if (!material) {
        return Color(0, 0, 0); // Default black color
    }

    // Initialize diffuse and specular components
    Color diffuse(0, 0, 0);
    Color specular(0, 0, 0);

    // Get the normal vector at the intersection point
    Vec3 N = shape.getNormal(intersectionPoint);

    // Handle texture, if present
    if (shape.getTexture()) {
        Vec2 textureCoord = shape.getTextureCoordinate(intersectionPoint);
        Color textureColor = shape.getTexture()->getPixel(textureCoord.getX(), textureCoord.getY());
        material->setOd(textureColor);
    }

    // Ambient component
    Color ambient = material->getOd() * material->getKa();

    // Calculate diffuse and specular components for each light
    for (AbstractLight* light : scene.getLights()) {
        // Determine shadow factor
        double sF = light->calculateShadowFactor(intersectionPoint, scene);

        // Diffuse component
        diffuse += light->calculateDiffuse(*material, intersectionPoint, N) * sF;

        // Specular component
        specular += light->calculateSpecular(*material, intersectionPoint, N, ray.getDirection()) * sF;
    }

    // Combine ambient, diffuse, and specular components
    Color phongColor = ambient + (diffuse + specular);

    // Reflective shading
    //if (material->getAlpha() > 0.0) { // Reflectivity > 0
    //    Vec3 reflectionDirection = ray.getDirection().reflect(N).normal();
    //    Ray reflectionRay(intersectionPoint + reflectionDirection * 1e-4, reflectionDirection); // Offset to avoid self-intersection
    //    Color reflectedColor = Rendering::traceRay(reflectionRay, scene);
    //    phongColor = phongColor * (1.0 - material->getAlpha()) + reflectedColor * material->getAlpha();
    //}

    //// Refractive shading
    //if (material->getEta() > 1.0) { // Refractive index > 1 indicates transparency
    //    double eta = material->getEta();
    //    Vec3 I = ray.getDirection().normal();
    //    double cosThetaI = -N.dot(I);
    //    bool entering = cosThetaI > 0;

    //    if (!entering) {
    //        N = -1 * N;
    //        eta = 1.0 / eta; // Invert eta for exiting rays
    //    }
    //
    //double sin2ThetaT = eta * eta * (1.0 - cosThetaI * cosThetaI);

    //    if (sin2ThetaT <= 1.0) { // No total internal reflection
    //        double cosThetaT = sqrt(1.0 - sin2ThetaT);
    //        Vec3 refractedDirection = eta * I + (eta * cosThetaI - cosThetaT) * N;
    //        refractedDirection = refractedDirection.normal();

    //        Ray refractionRay(intersectionPoint + refractedDirection * 1e-4, refractedDirection);
    //        Color refractedColor = Rendering::traceRay(refractionRay, scene);

    //        phongColor = phongColor * (1.0 - material->getAlpha()) + refractedColor * material->getAlpha();
    //    }
    //}

    //// Depth cueing (optional)
    //if (scene.getDepthCue()) {
    //    double depthFactor = scene.getDepthCue()->calculateDepthCueingFactor((ray.getOrigin() - intersectionPoint).length());
    //    //phongColor = phongColor * depthFactor + scene.getDepthCue()->getDepthCueColor() * (1.0 - depthFactor);
    //}

    return phongColor;
}
