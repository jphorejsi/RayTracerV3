#pragma once
#include "color.h"
#include "vec.h"
#include "ray.h"
#include "scene.h"
#include "camera.h"

class Renderer {
    int imageWidth = 256;
    int imageHeight = 256;
    int samples = 1;
    int maxDepth = 10;

public:
    // Constructor
    Renderer() = default;
    Renderer(int imageWidth, int imageHeight);

    // Setters
    void setSize(int imageWidth, int imageHeight);
    void setSamples(int samples);

    // Other methods
    static Color traceRay(const Ray& ray, const Scene& scene, int maxDepth);
    void render(Scene& scene, Camera& camera);
};