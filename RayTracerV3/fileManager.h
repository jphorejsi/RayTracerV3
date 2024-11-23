#pragma once

#include <fstream>
#include <sstream>
#include "camera.h"
#include "scene.h"
#include "ImageSize.h"

class FileReader {
    void processEye(std::istringstream& iss, Camera& camera);
    void processViewDir(std::istringstream& iss, Camera& camera);
    void processUpDir(std::istringstream& iss, Camera& camera);
    void processHfov(std::istringstream& iss, Camera& camera);
    void processBackgroundColor(std::istringstream& iss, SceneBuilder& sceneBuilder);
    void processImageSize(std::istringstream& iss, ImageSize& imageSize);
    void processMaterial(std::istringstream& iss, SceneBuilder& sceneBuilder);
    void processTexture(std::istringstream& iss, SceneBuilder& sceneBuilder);
    void processNormalMap(std::istringstream& iss, SceneBuilder& sceneBuilder);
    void processSphere(std::istringstream& iss, SceneBuilder& sceneBuilder);
    void processLight(std::istringstream& iss, SceneBuilder& sceneBuilder);
    void processAttributeLight(std::istringstream& iss, SceneBuilder& sceneBuilder);
    void processVertex(std::istringstream& iss, SceneBuilder& sceneBuilder);
    void processVertexNormal(std::istringstream& iss, SceneBuilder& sceneBuilder);
    void processTextureCoordinate(std::istringstream& iss, SceneBuilder& sceneBuilder);
    void processTriangle(std::string& line, SceneBuilder& sceneBuilder);
    void processDepthCue(std::istringstream& iss, SceneBuilder& sceneBuilder);

public:
    int readFile(const std::string& filename, Camera& camera, SceneBuilder& sceneBuilder, ImageSize& imageSize);
};


class FileWriter {
public:
    FileWriter() = default;

    static std::string createPPMFile(const std::string& filename, const ImageSize& imageSize);
};
