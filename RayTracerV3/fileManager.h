#pragma once

#include <sstream>
#include <string>
#include "camera.h"
#include "scene.h"
#include "renderer.h"
#include "materials.h"
#include "textures.h"

class FileReader {
    std::shared_ptr<Material> currentMaterial = nullptr;
    std::vector<Vec3> vertices;
    std::vector<Vec3> VertexNormals;
    std::vector<Vec2> textureCoordinates;

public:
    static void processEye(std::istringstream& iss, Camera& camera);
    static void processViewDir(std::istringstream& iss, Camera& camera);
    static void processUpDir(std::istringstream& iss, Camera& camera);
    static void processHfov(std::istringstream& iss, Camera& camera);
    static void processBackgroundColor(std::istringstream& iss, Scene& scene);
    static void processImageSize(std::istringstream& iss, Renderer& system);
    
    void processMaterial(std::istringstream& iss);
    
    void processTexture(std::istringstream& iss);
    
    void processNormalMap(std::istringstream& iss);
    
    void processSphere(std::istringstream& iss, Scene& scene);
    
    static void processLight(std::istringstream& iss, Scene& scene);
    static void processAttributeLight(std::istringstream& iss, Scene& scene);

    void processVertex(std::istringstream& iss);
    void processVertexNormal(std::istringstream& iss);
    void processTextureCoordinate(std::istringstream& iss);

    void processTriangle(std::string& line, Scene& scene);

    static void processDepthCue(std::istringstream& iss, Scene& scene);
    void readFile(const std::string& filename, Camera& camera, Scene& scene, Renderer& system);
};
