//#pragma once
//#include <fstream>
//#include <sstream>
//#include "camera.h"
//#include "scene.h"
//#include "ImageSize.h"
//
//class FileReader {
//public:
//    // Constructor
//    FileReader() = default;
//
//    // Static method to read file
//    static int readFile(const std::string& filename, CameraBuilder& cameraBuilder, SceneBuilder& sceneBuilder, ImageSize& imageSize);
//
//private:
//    // Static helper methods
//    static void processEye(std::istringstream& iss, CameraBuilder& cameraBuilder);
//    static void processViewDir(std::istringstream& iss, CameraBuilder& cameraBuilder);
//    static void processUpDir(std::istringstream& iss, CameraBuilder& cameraBuilder);
//    static void processHfov(std::istringstream& iss, CameraBuilder& cameraBuilder);
//    static void processBackgroundColor(std::istringstream& iss, SceneBuilder& sceneBuilder);
//    static void processImageSize(std::istringstream& iss, ImageSize& imageSize);
//    static void processMaterial(std::istringstream& iss, SceneBuilder& sceneBuilder);
//    static void processSphere(std::istringstream& iss, SceneBuilder& sceneBuilder);
//    static void processLight(std::istringstream& iss, SceneBuilder& sceneBuilder);
//    static void processAttLight(std::istringstream& iss, SceneBuilder& sceneBuilder);
//    static void processTexture(std::istringstream& iss, SceneBuilder& sceneBuilder);
//    static void processVertex(std::istringstream& iss, SceneBuilder& sceneBuilder);
//    static void processVertexNormal(std::istringstream& iss, SceneBuilder& sceneBuilder);
//    static void processTextureCoordinate(std::istringstream& iss, SceneBuilder& sceneBuilder);
//    static void processTriangle(std::string& line, SceneBuilder& sceneBuilder);
//};
//
//
//
//class FileWriter {
//public:
//    // Constructor
//    FileWriter() = default;
//
//    // Other methods
//    static std::string createPPMFile(std::string filename, const ImageSize& imageSize);
//    //void RenderToPPM(const std::string& filename, const ImageSize& imageSize, const ViewFrustum& viewFrustum, const Camera& camera, const Scene& scene);
//};
