#include "renderer.h"
#include <stdexcept>

Renderer::Renderer(int imageWidth, int imageHeight) {
    if (imageWidth < 1) {
        throw std::invalid_argument("imageWidth must be greater than 1.");
    }
    if (imageHeight < 1) {
        throw std::invalid_argument("imageHeight must be greater than 1.");
    }
    if (samples < 1) {
        throw std::invalid_argument("Samples must be greater than 1.");
    }
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;
    this->samples = samples;
}

const Shape* findClosestIntersectedShape(const Ray& ray, const Scene& scene, Vec3& intersectionPoint) {
    const Shape* closestShape = nullptr;
    double closestDistance = std::numeric_limits<double>::max();

    // Get all intersected leaf nodes using the KDTree's method
    std::vector<BVHNode*> intersectedNodes = scene.getBVHRoot()->findAllIntersectedLeafNodes(ray);

    // Iterate through all intersected nodes and check for ray-shape intersections
    for (const BVHNode* node : intersectedNodes) {
        for (const Shape* shape : node->getShapes()) {  // Explicit type for shape
            Vec3 currentIntersection;
            if (shape->intersects(ray, currentIntersection)) {
                double distance = (currentIntersection - ray.origin).length();
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestShape = shape;
                    intersectionPoint = currentIntersection;
                }
            }
        }
    }
    return closestShape;
}


Color Renderer::traceRay(const Ray& ray, const Scene& scene, int maxDepth) {
    // Base case: Black if recursion limit reached
    if (maxDepth < 0) return Color(0, 0, 0);

    Vec3 intersectionPoint;

    const Shape* closestShape = findClosestIntersectedShape(ray, scene, intersectionPoint);
    if (!closestShape) return scene.getBackgroundColor();  // No intersection

    return closestShape->getMaterial().shade(ray, intersectionPoint, scene, *closestShape);
}

void Renderer::render(Scene& scene, Camera& camera) {
    std::ofstream outFile("image.ppm");

    // Create viewing frustum
    camera.createFrustrum(imageWidth, imageHeight);

    // Calculate delta values
    Vec3 deltaH = (camera.getVFUpperRight() - camera.getVFUpperLeft()) / static_cast<double>(imageWidth - 1);
    Vec3 deltaV = (camera.getVFLowerLeft() - camera.getVFUpperLeft()) / static_cast<double>(imageHeight - 1);

    // Build BVH
    scene.buildBVH();

    // Write PPM file header
    outFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    // Calculate sampling details
    int sqrtSamples = static_cast<int>(std::sqrt(samples));
    samples = sqrtSamples * sqrtSamples; // Ensure total number of samples is correct
    double sampleStep = 1.0 / sqrtSamples;

    for (int j = 0; j < imageHeight; j++) {
        for (int i = 0; i < imageWidth; i++) {
            if (i != 233 || j != 212) {
                continue;
            }

            double accumulatedR = 0.0;
            double accumulatedG = 0.0;
            double accumulatedB = 0.0;

            // Super-sampling: generate rays for each sub-pixel sample
            for (int sy = 0; sy < sqrtSamples; sy++) {
                for (int sx = 0; sx < sqrtSamples; sx++) {
                    double offsetX = 0.0;
                    double offsetY = 0.0;

                    if (samples > 1) {
                        // Only calculate offsets if there are multiple samples
                        offsetX = (sx + 0.5) * sampleStep;
                        offsetY = (sy + 0.5) * sampleStep;
                    }

                    // Calculate sub-pixel position
                    Vec3 subPixelPosition = camera.getVFUpperLeft() + deltaH * (i + offsetX) + deltaV * (j + offsetY);

                    // Create a ray from the camera's eye position to the sub-pixel position
                    Ray ray(camera.eyePosition, (subPixelPosition - camera.eyePosition));

                    // Trace the ray and accumulate color
                    Color traced = traceRay(ray, scene, maxDepth);
                    accumulatedR += traced.getR();
                    accumulatedG += traced.getG();
                    accumulatedB += traced.getB();
                }
            }


            // Average the accumulated color
            accumulatedR /= static_cast<double>(samples);
            accumulatedG /= static_cast<double>(samples);
            accumulatedB /= static_cast<double>(samples);
            Color output(accumulatedR, accumulatedG, accumulatedB);

            // Write pixel color to file
            outFile << static_cast<int>(output.getR() * 255) << " " << static_cast<int>(output.getG() *255) << " " << static_cast<int>(output.getB() * 255) << "\n";
        }
    }
}
