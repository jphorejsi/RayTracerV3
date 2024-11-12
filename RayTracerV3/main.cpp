#include <iostream>
#include "camera.h"
#include "scene.h"
#include "imageSize.h"
#include "fileManager.h"
#include "viewFrustrum.h"
#include "kdtree.h"
#include "rendering.h"

#include <string>

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

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
    KDTreeNode* kdTreeRoot = new KDTreeNode(sceneBuilder.getShapes(), 0); // Use the new constructor to build the KDTree
    sceneBuilder.setKDRoot(kdTreeRoot);  // Set the root node in the scene

    // Complete scene
    Scene scene = sceneBuilder.build();

    // Complete camera
    Camera camera = cameraBuilder.build();

    // Step 3: Prepare viewing window
    ViewFrustrum viewFrustrum(camera, imageSize);

    // Step 4: Build output file
    FileWriter::createPPMFile(outputFilename, imageSize);

    // get deltas for stepping through the view frustum
    Vec3 deltaH = (viewFrustrum.getUpperRight() - viewFrustrum.getUpperLeft()) / (float)(imageSize.getWidth() - 1);
    Vec3 deltaV = (viewFrustrum.getLowerLeft() - viewFrustrum.getUpperLeft()) / (float)(imageSize.getHeight() - 1);

    // Iterate over each pixel to cast rays
    for (int j = 0; j < imageSize.getHeight(); j++) {
        for (int i = 0; i < imageSize.getWidth(); i++) {
            // get the pixel position on the view frustum
            Vec3 pixelPosition = viewFrustrum.getUpperLeft() + deltaH * i + deltaV * j;
            // Create a ray from the camera's eye position to the pixel position
            Ray ray(camera.getEyePosition(), (pixelPosition - camera.getEyePosition()).normal());
            // The ray is now set up for this pixel; proceed with intersection tests and shading as needed
            Color color = renderer.traceRay(ray, scene, 0);
        }
    }

    // Cleanup
    return 0;
}
