#pragma once

#include <fstream>
#include <sstream>
#include "camera.h"
#include "scene.h"
#include "ImageSize.h"

class FileReader {
    IMaterial* currentMaterial = nullptr;
    Texture* currentTexture = nullptr;
    NormalMap* currentNormalMap = nullptr;
    std::vector<Vec3*> vertices;
    std::vector<Vec3*> vertexNormals;
    std::vector<Vec2*> textureCoordinates;

    void processEye(std::istringstream& iss, CameraBuilder& cameraBuilder);
    void processViewDir(std::istringstream& iss, CameraBuilder& cameraBuilder);
    void processUpDir(std::istringstream& iss, CameraBuilder& cameraBuilder);
    void processHfov(std::istringstream& iss, CameraBuilder& cameraBuilder);
    void processBackgroundColor(std::istringstream& iss, SceneBuilder& sceneBuilder);
    void processImageSize(std::istringstream& iss, ImageSize& imageSize);
    void processMaterial(std::istringstream& iss);
    //void processTexture(std::istringstream& iss, Texture*& currentTexture);
    //void processNormalMap(std::istringstream& iss, NormalMap*& currentNormalMap);
    void processSphere(std::istringstream& iss, SceneBuilder& sceneBuilder);
    void processLight(std::istringstream& iss, SceneBuilder& sceneBuilder);
    void processVertex(std::istringstream& iss);
    void processVertexNormal(std::istringstream& iss);
    void processTextureCoordinate(std::istringstream& iss);
    //void processTriangle(std::string& line, SceneBuilder& sceneBuilder);

public:
    int readFile(const std::string& filename, CameraBuilder& cameraBuilder, SceneBuilder& sceneBuilder, ImageSize& imageSize);
};


class FileWriter {
public:
    FileWriter() = default;

    static std::string createPPMFile(const std::string& filename, const ImageSize& imageSize);
};
