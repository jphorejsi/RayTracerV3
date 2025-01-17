#pragma once
#include "color.h"
#include "vec.h"
#include "ray.h"
#include "scene.h"
#include "camera.h"

class Renderer {
    int imageWidth;
    int imageHeight;
    int samples = 1;
    int maxDepth = 10;

public:
    // Constructor
    Renderer(int imageWidth, int imageHeight);

    // Setters
    void setSamples(int samples) { 
        if (samples < 1) {
            return;
        }
        else this->samples = samples;
    }

    // Other methods
    static Color traceRay(const Ray& ray, const Scene& scene, int maxDepth);
    void render(Scene& scene, Camera& camera);
};