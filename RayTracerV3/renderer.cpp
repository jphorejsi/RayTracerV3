#include "renderer.h"
#include <stdexcept>
#include <chrono> // For timing
#include <iostream>

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


Color Renderer::traceRay(const Ray& ray, const Scene& scene, int maxDepth) {
    // Base case: Black if recursion limit reached
    if (maxDepth < 0) return Color(0, 0, 0);

    Vec3 intersectionPoint;

    const Shape* closestShape = scene.getBVHRoot()->findClosestIntersectedShape(ray, intersectionPoint);
    if (!closestShape) return scene.getBackgroundColor();  // No intersection

    return closestShape->getMaterial().shade(ray, intersectionPoint, scene, *closestShape, maxDepth);
}

void Renderer::render(Scene& scene, Camera& camera) {
    std::ofstream outFile("image.ppm");

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

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
            outFile << static_cast<int>(output.getR() * 255) << " " << static_cast<int>(output.getG() * 255) << " " << static_cast<int>(output.getB() * 255) << "\n";
        }
    }

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Print the time taken
    std::cout << "Render completed in " << duration.count() << " milliseconds.\n";
}

