#include <iostream>

#include "camera.h"
#include "scene.h"
#include "imageSize.h"
#include "fileManager.h"
#include "viewFrustrum.h"

int main() {
    CameraType camera;
    SceneType scene;
    ImageSizeType imageSize;
    FileReaderType fReader;
    FileWriterType fWriter;
    ViewFrustrumType vf;

    std::string filename = "test.txt";

    // Step 1: Read scene data
    fReader.readFile(filename, camera, scene, imageSize);

    // Step 3: Build spatial structure (KDTree)
    //KDTreeNode* kdTreeRoot = new KDTreeNode(scene.getShapes(), 0); // Use the new constructor to build the KDTree
    //scene.setKDRoot(kdTreeRoot);  // Set the root node in the scene

    //// Step 4: Prepare viewing window
    //ViewFrustrumType viewFrustrum(camera, imageSize); // Build viewing window

    //// create output file
    //fWriter.createPPMFile(filename, imageSize);
    //fWriter.RenderToPPM(filename, imageSize, viewFrustrum, camera, scene);

    // Cleanup if necessary
    //delete kdTreeRoot;

    return 0;
}
