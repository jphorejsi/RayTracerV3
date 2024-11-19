//#include <iostream>
//#include "camera.h"
//#include "scene.h"
//#include "imageSize.h"
//#include "fileManager.h"
//#include "viewFrustrum.h"
//#include "bvh.h"
//#include "rendering.h"
//
//#include <string>
//#include <algorithm>
//
//#define _CRTDBG_MAP_ALLOC
//#include <cstdlib>
//#include <crtdbg.h>
//
//template <typename T>
//T clamp(T value, T min, T max) {
//    if (value < min) return min;
//    if (value > max) return max;
//    return value;
//}
//
//
//int main() {
//    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//    CameraBuilder cameraBuilder;
//    SceneBuilder sceneBuilder;
//    Rendering renderer;
//    ImageSize imageSize;
//
//    std::string inputFilename = "test.txt";
//    std::string outputFilename = "output.ppm";
//
//    // Step 1: Read scene data
//    FileReader fw;
//    fw.readFile(inputFilename, cameraBuilder, sceneBuilder, imageSize);
//
//    // Step 2: Build spatial structure (KDTree)
//    BVHNode* kdTreeRoot = new BVHNode(sceneBuilder.getShapes(), 0); // Use the new constructor to build the KDTree
//    sceneBuilder.setBVHRoot(kdTreeRoot);  // Set the root node in the scene
//
//    // Complete scene
//    Scene scene = sceneBuilder.build();
//
//    // Complete camera
//    Camera camera = cameraBuilder.build();
//
//    // Step 3: Prepare viewing window
//    ViewFrustrum viewFrustrum(camera, imageSize);
//
//    // Step 4: Build output file
//    //FileWriter::createPPMFile(outputFilename, imageSize);
//    std::ofstream outputFile(outputFilename, std::ios::out | std::ios::binary);
//    outputFile << "P3\n" << imageSize.getWidth() << " " << imageSize.getHeight() << "\n255\n";
//
//    // get deltas for stepping through the view frustum
//    Vec3 deltaH = (viewFrustrum.getUpperRight() - viewFrustrum.getUpperLeft()) / (float)(imageSize.getWidth() - 1);
//    Vec3 deltaV = (viewFrustrum.getLowerLeft() - viewFrustrum.getUpperLeft()) / (float)(imageSize.getHeight() - 1);
//
//    // Iterate over each pixel to cast rays
//    for (int j = 0; j < imageSize.getHeight(); j++) {
//        for (int i = 0; i < imageSize.getWidth(); i++) {
//            // get the pixel position on the view frustum
//            Vec3 pixelPosition = viewFrustrum.getUpperLeft() + deltaH * i + deltaV * j;
//            // Create a ray from the camera's eye position to the pixel position
//            Ray ray(camera.getEyePosition(), (pixelPosition - camera.getEyePosition()).normal());
//            // The ray is now set up for this pixel; proceed with intersection tests and shading as needed
//            Color color = renderer.traceRay(ray, scene, 0);
//
//
//            int r = static_cast<int>(clamp(color.getR() * 255, 0.0f, 255.0f));
//            int g = static_cast<int>(clamp(color.getG() * 255, 0.0f, 255.0f));
//            int b = static_cast<int>(clamp(color.getB() * 255, 0.0f, 255.0f));
//
//
//            // Append the color to the output file
//            outputFile << r << " " << g << " " << b << " ";
//        }
//    }
//
//    outputFile.close(); // Close the file after writing all pixels
//
//    // Cleanup
//    return 0;
//}


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono> // Include chrono for timing
#include "camera.h"
#include "scene.h"
#include "imageSize.h"
#include "fileManager.h"
#include "viewFrustrum.h"
#include "bvh.h"
#include "rendering.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

template <typename T>
T clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Shared storage for each row's color data
std::vector<std::string> imageRows;

void renderSection(int startRow, int endRow, const ImageSize& imageSize, const ViewFrustrum& viewFrustrum,
    const Camera& camera, const Scene& scene, Rendering& renderer) {
    Vec3 deltaH = (viewFrustrum.getUpperRight() - viewFrustrum.getUpperLeft()) / (float)(imageSize.getWidth() - 1);
    Vec3 deltaV = (viewFrustrum.getLowerLeft() - viewFrustrum.getUpperLeft()) / (float)(imageSize.getHeight() - 1);

    for (int j = startRow; j < endRow; j++) {
        std::string row;
        for (int i = 0; i < imageSize.getWidth(); i++) {
            Vec3 pixelPosition = viewFrustrum.getUpperLeft() + deltaH * i + deltaV * j;
            Ray ray(camera.getEyePosition(), (pixelPosition - camera.getEyePosition()).normal());
            Color color = renderer.traceRay(ray, scene, 0);

            int r = static_cast<int>(clamp(color.getR() * 255, 0.0f, 255.0f));
            int g = static_cast<int>(clamp(color.getG() * 255, 0.0f, 255.0f));
            int b = static_cast<int>(clamp(color.getB() * 255, 0.0f, 255.0f));

            row += std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + " ";
        }
        imageRows[j] = row; // Store the row in its designated position
    }
}

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    CameraBuilder cameraBuilder;
    SceneBuilder sceneBuilder;
    Rendering renderer;
    ImageSize imageSize;

    std::string inputFilename = "test.txt";
    std::string outputFilename = "output.ppm";

    // Step 1: Read scene data
    FileReader fw;
    fw.readFile(inputFilename, cameraBuilder, sceneBuilder, imageSize);

    // Step 2: Build spatial structure (KDTree)
    BVHNode* BVHRoot = new BVHNode();
    BVHRoot->buildBVH(sceneBuilder.getShapes(), 2);

    sceneBuilder.setBVHRoot(BVHRoot);

    // Complete scene and camera
    Scene scene = sceneBuilder.build();
    Camera camera = cameraBuilder.build();

    // Step 3: Prepare viewing window
    ViewFrustrum viewFrustrum(camera, imageSize);

    // Initialize the shared row storage
    imageRows.resize(imageSize.getHeight());

    // Determine the number of threads to use
    int numThreads = std::thread::hardware_concurrency();
    int rowsPerThread = imageSize.getHeight() / numThreads;
    int remainingRows = imageSize.getHeight() % numThreads;

    std::cout << "Starting rendering...\n";

    // Start timing
    auto startTime = std::chrono::high_resolution_clock::now();

    // Launch threads
    std::vector<std::thread> threads;
    int currentRow = 0;
    for (int t = 0; t < numThreads; ++t) {
        int startRow = currentRow;
        int endRow = startRow + rowsPerThread + (t < remainingRows ? 1 : 0);
        threads.emplace_back(renderSection, startRow, endRow, std::cref(imageSize), std::cref(viewFrustrum),
            std::cref(camera), std::cref(scene), std::ref(renderer));
        currentRow = endRow;
    }

    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }

    // Stop timing
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedSeconds = endTime - startTime;

    std::cout << "Rendering completed in " << elapsedSeconds.count() << " seconds.\n";

    // Write the image rows to the output file in correct order
    std::ofstream outputFile(outputFilename, std::ios::out | std::ios::binary);
    outputFile << "P3\n" << imageSize.getWidth() << " " << imageSize.getHeight() << "\n255\n";
    for (const auto& row : imageRows) {
        outputFile << row << "\n";
    }
    outputFile.close();

    return 0;
}
