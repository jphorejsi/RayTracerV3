#include "fileManager.h"

#include "vecFactory.h"
#include "shapeFactory.h"
#include "lightFactory.h"
#include "materialFactory.h"
#include "shapes.h"
#include "materials.h"
#include "lights.h"
#include "vec.h"

#include <unordered_map>
#include <stdexcept>


int FileReader::readFile(const std::string& filename, CameraBuilder& cameraBuilder, SceneBuilder& sceneBuilder, ImageSize& imageSize) {
    // Open input file
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }
    // Create unorded map of required parameters
    std::unordered_map<std::string, bool> requiredParams = {
        {"eye", false}, {"viewdir", false}, {"updir", false},
        {"hfov", false}, {"bkgcolor", false}
    };
    // Read file line by line
    std::string line;
    while (std::getline(inputFile, line)) {
        // Retrieve keyword as first word in line
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;
        // Check for keyword and process based on this keyword
        if (keyword == "eye") {
            processEye(iss, cameraBuilder);
            requiredParams["eye"] = true;
        }
        else if (keyword == "viewdir") {
            processViewDir(iss, cameraBuilder);
            requiredParams["viewdir"] = true;
        }
        else if (keyword == "updir") {
            processUpDir(iss, cameraBuilder);
            requiredParams["updir"] = true;
        }
        else if (keyword == "hfov") {
            processHfov(iss, cameraBuilder);
            requiredParams["hfov"] = true;
        }
        else if (keyword == "bkgcolor") {
            processBackgroundColor(iss, sceneBuilder);
            requiredParams["bkgcolor"] = true;
        }
        else if (keyword == "imsize") {
            processImageSize(iss, imageSize);
        }
        else if (keyword == "mtlcolor") {
            processMaterial(iss, sceneBuilder);
        }
        else if (keyword == "texture") {
            processTexture(iss, sceneBuilder);
        }
        else if (keyword == "sphere") {
            processSphere(iss, sceneBuilder);
        }
        else if (keyword == "light") {
            processLight(iss, sceneBuilder);
        }
        else if (keyword == "v") {
            processVertex(iss, sceneBuilder);
        }
        else if (keyword == "vn") {
            processVertexNormal(iss, sceneBuilder);
        }
        else if (keyword == "vt") {
            processTextureCoordinate(iss, sceneBuilder);
        }
        else if (keyword == "f") {
            processTriangle(line, sceneBuilder);
        }
    }
    // Check required parameters were provided
    for (const auto& param : requiredParams) {
        if (!param.second) {
            throw std::runtime_error("Missing required parameter: " + param.first);
        }
    }
    return 0;
}

// Read eye into cameraBuilder
void FileReader::processEye(std::istringstream& iss, CameraBuilder& cameraBuilder) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing eye position coordinates.");
    }
    Vec3 eyePosition(x, y, z);
    cameraBuilder.setEyePosition(eyePosition);
}

// read view direction into cameraBuilder
void FileReader::processViewDir(std::istringstream& iss, CameraBuilder& cameraBuilder) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing view direction coordinates.");
    }
    Vec3 viewDirection(x, y, z);
    cameraBuilder.setViewDirection(viewDirection);
}

// Read up direction into cameraBuilder
void FileReader::processUpDir(std::istringstream& iss, CameraBuilder& cameraBuilder) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing up direction coordinates.");
    }
    Vec3 upDirection(x, y, z);
    cameraBuilder.setUpDirection(upDirection);
}

// Read horizontal fov into cameraBuilder
void FileReader::processHfov(std::istringstream& iss, CameraBuilder& cameraBuilder) {
    float fov;
    if (!(iss >> fov)) {
        throw std::runtime_error("Error: Invalid or missing horizontal field of view.");
    }
    cameraBuilder.setHorizontalFOV(fov);
}

// Read background color into sceneBuilder
void FileReader::processBackgroundColor(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    float r, g, b;
    if (!(iss >> r >> g >> b)) {
        throw std::runtime_error("Error: Invalid or missing background color values.");
    }
    Color backgroundColor(r, g, b);
    sceneBuilder.setBackgroundColor(backgroundColor);
}

// Read image size into imageSize
void FileReader::processImageSize(std::istringstream& iss, ImageSize& imageSize) {
    int width, height;
    if (!(iss >> width >> height)) {
        throw std::runtime_error("Error: Invalid or missing image size.");
    }
    imageSize.setSize(width, height);
}

// Read material into sceneBuilder
void FileReader::processMaterial(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    float odr, odg, odb, osr, osg, osb, ka, kd, ks, n;
    if (!(iss >> odr >> odg >> odb >> osr >> osg >> osb >> ka >> kd >> ks >> n)) {
        throw std::runtime_error("Error: Invalid or missing material parameters.");
    }
    Color diffuse(odr, odg, odb);
    Color specular(osr, osg, osb);
    IMaterial* material = MaterialFactory::createPhongMaterial(diffuse, specular, ka, kd, ks, n);
    sceneBuilder.addMaterial(material);  // Add material to SceneBuilder's vector
}

// Read texture into currentTexture
void FileReader::processTexture(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    std::string filename;
    if (!(iss >> filename) || filename.empty()) {
        throw std::runtime_error("Error: Invalid or missing filename for texture.");
    }
    sceneBuilder.addTexture(new Texture(filename));
}

// Read sphere into sceneBuilder
void FileReader::processSphere(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    float x, y, z, r;
    if (!(iss >> x >> y >> z >> r)) {
        throw std::runtime_error("Error: Invalid or missing sphere parameters.");
    }
    Vec3 position(x, y, z);
    AbstractShape* sphere = nullptr;

    IMaterial* currentMaterial = nullptr;
    if (!sceneBuilder.getMaterials().empty()) {
        currentMaterial = sceneBuilder.getMaterials().back();
    }
    Texture* currentTexture = nullptr;
    if (!sceneBuilder.getTextures().empty()) {
        currentTexture = sceneBuilder.getTextures().back();
    }
    NormalMap* currentNormalMap = nullptr;
    if (!sceneBuilder.getNormalMaps().empty()) {
        currentNormalMap = sceneBuilder.getNormalMaps().back();
    }

    if (currentMaterial && currentTexture && currentNormalMap) {
        sphere = SphereFactory::createNormalMappedSphere(position, r, currentMaterial, currentTexture, currentNormalMap);
    }
    else if (currentMaterial && currentTexture) {
        sphere = SphereFactory::createTexturedSphere(position, r, currentMaterial, currentTexture);
    }
    else if (currentMaterial) {
        sphere = SphereFactory::createSphere(position, r, currentMaterial);
    }
    else {
        throw std::runtime_error("Error: Material is required to create a sphere.");
    }
    sceneBuilder.addShape(sphere);
}

// Read light into sceneBuilder
void FileReader::processLight(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    float x, y, z, r, g, b;
    int type;
    if (!(iss >> x >> y >> z >> type >> r >> g >> b)) {
        throw std::runtime_error("Error: Invalid or missing light parameters.");
    }
    Color color(r, g, b);
    Vec3 position(x, y, z);
    AbstractLight* light = nullptr;
    if (type == 1) {
        light = LightFactory::createPointLight(position, color);
    }
    else if (type == 0) {
        light = LightFactory::createDirectionalLight(position, color);
    }
    else {
        throw std::runtime_error("Error: light type is not 0 or 1.");
    }
    sceneBuilder.addLight(light);
}

// Read vertex into verticies
void FileReader::processVertex(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing vertex coordinates.");
    }
    Vec3* vertex = VertexFactory::createVertex(x, y, z);
    sceneBuilder.addVertex(vertex);
}

// Read vertex normal into vertexNormals
void FileReader::processVertexNormal(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing vertex normal coordinates.");
    }
    Vec3* vertexNormal = VertexFactory::createVertexNormal(x, y, z);
    sceneBuilder.addVertexNormal(vertexNormal);
}

// Read texture coordinate into textureCoordinates
void FileReader::processTextureCoordinate(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    float u, v;
    if (!(iss >> u >> v)) {
        throw std::runtime_error("Error: Invalid or missing texture coordinates.");
    }
    Vec2* textureCoordinate = VertexFactory::createTextureCoordinate(u, v);
    sceneBuilder.addTextureCoordinate(textureCoordinate);
}

// Read triangle into sceneBuilder
void FileReader::processTriangle(std::string& line, SceneBuilder& sceneBuilder) {
    int vertexAIndex, vertexBIndex, vertexCIndex;
    int vertexANormalIndex, vertexBNormalIndex, vertexCNormalIndex;
    int textureCoordinateAIndex, textureCoordinateBIndex, textureCoordinateCIndex;
    AbstractShape* triangle = nullptr;

    IMaterial* currentMaterial = nullptr;
    if (!sceneBuilder.getMaterials().empty()) {
        currentMaterial = sceneBuilder.getMaterials().back();
    }
    Texture* currentTexture = nullptr;
    if (!sceneBuilder.getTextures().empty()) {
        currentTexture = sceneBuilder.getTextures().back();
    }
    NormalMap* currentNormalMap = nullptr;
    if (!sceneBuilder.getNormalMaps().empty()) {
        currentNormalMap = sceneBuilder.getNormalMaps().back();
    }

    if (sscanf_s(line.c_str(), "f %d %d %d", &vertexAIndex, &vertexBIndex, &vertexCIndex) == 3) {
        triangle = TriangleFactory::createTriangle(sceneBuilder.getVertices()[vertexAIndex], sceneBuilder.getVertices()[vertexBIndex], sceneBuilder.getVertices()[vertexCIndex], currentMaterial);
    }
    else if (sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", &vertexAIndex, &vertexANormalIndex, &vertexBIndex, &vertexBNormalIndex, &vertexCIndex, &vertexCNormalIndex) == 6) {
        triangle = TriangleFactory::createSmoothShadedTriangle(sceneBuilder.getVertices()[vertexAIndex], sceneBuilder.getVertexNormals()[vertexANormalIndex], sceneBuilder.getVertices()[vertexBIndex], sceneBuilder.getVertexNormals()[vertexBNormalIndex], sceneBuilder.getVertices()[vertexCIndex], sceneBuilder.getVertexNormals()[vertexCNormalIndex], currentMaterial);
    }
    else if (sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d", &vertexAIndex, &textureCoordinateAIndex, &vertexBIndex, &textureCoordinateBIndex, &vertexCIndex, &textureCoordinateCIndex) == 6) {
        triangle = TriangleFactory::createTexturedTriangle(sceneBuilder.getVertices()[vertexAIndex], sceneBuilder.getTextureCoordinates()[textureCoordinateAIndex], sceneBuilder.getVertices()[vertexBIndex], sceneBuilder.getTextureCoordinates()[textureCoordinateBIndex], sceneBuilder.getVertices()[vertexCIndex], sceneBuilder.getTextureCoordinates()[textureCoordinateCIndex], currentMaterial, currentTexture);
    }
    else if (sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertexAIndex, &textureCoordinateAIndex, &vertexANormalIndex, &vertexBIndex, &textureCoordinateBIndex, &vertexBNormalIndex, &vertexCIndex, &textureCoordinateCIndex, &vertexCNormalIndex) == 9) {
        triangle = TriangleFactory::createSmoothShadedTexturedTriangle(sceneBuilder.getVertices()[vertexAIndex], sceneBuilder.getTextureCoordinates()[textureCoordinateAIndex], sceneBuilder.getVertexNormals()[vertexANormalIndex], sceneBuilder.getVertices()[vertexBIndex], sceneBuilder.getTextureCoordinates()[textureCoordinateBIndex], sceneBuilder.getVertexNormals()[vertexBNormalIndex], sceneBuilder.getVertices()[vertexCIndex], sceneBuilder.getTextureCoordinates()[textureCoordinateCIndex], sceneBuilder.getVertexNormals()[vertexCNormalIndex], currentMaterial, currentTexture);
    }
    else {
        throw std::runtime_error("Error: Triangle parameters are invalid.");
    }
    sceneBuilder.addShape(triangle);
}

// Create output ppm file based on input file name
std::string FileWriter::createPPMFile(const std::string& filename, const ImageSize& imageSize) {
    std::string ppmFilename = filename.substr(0, filename.find_last_of('.')) + ".ppm";
    std::ofstream outfile(ppmFilename);
    if (!outfile.is_open()) {
        throw std::runtime_error("Error: Unable to create PPM file: " + ppmFilename);
    }
    outfile << "P3\n" << imageSize.getWidth() << " " << imageSize.getHeight() << "\n255\n";
    return ppmFilename;
}
