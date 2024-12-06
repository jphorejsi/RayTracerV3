#include "fileManager.h"

#include "vecFactory.h"
#include "shapeFactory.h"
#include "lightFactory.h"
#include "materialFactory.h"
#include "shapes.h"
#include "materials.h"
#include "lights.h"
#include "vec.h"
#include "color.h"

#include <unordered_map>
#include <stdexcept>


int FileReader::readFile(const std::string& filename, Camera& camera, SceneBuilder& sceneBuilder, ImageSize& imageSize) {
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
            processEye(iss, camera);
            requiredParams["eye"] = true;
        }
        else if (keyword == "viewdir") {
            processViewDir(iss, camera);
            requiredParams["viewdir"] = true;
        }
        else if (keyword == "updir") {
            processUpDir(iss, camera);
            requiredParams["updir"] = true;
        }
        else if (keyword == "hfov") {
            processHfov(iss, camera);
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
        else if (keyword == "attlight") {
            processAttributeLight(iss, sceneBuilder);
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
        else if (keyword == "depthcueing") {
            processDepthCue(iss, sceneBuilder);
        }
        else if (keyword == "normalmap") {
            processNormalMap(iss, sceneBuilder);
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

// Read eye into camera
void FileReader::processEye(std::istringstream& iss, Camera& camera) {
    double x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing eye position coordinates.");
    }
    Vec3 eyePosition(x, y, z);
    camera.setEyePosition(eyePosition);
}

// read view direction into camera
void FileReader::processViewDir(std::istringstream& iss, Camera& camera) {
    double x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing view direction coordinates.");
    }
    Vec3 viewDirection(x, y, z);
    camera.setViewDirection(viewDirection);
}

// Read up direction into camera
void FileReader::processUpDir(std::istringstream& iss, Camera& camera) {
    double x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing up direction coordinates.");
    }
    Vec3 upDirection(x, y, z);
    camera.setUpDirection(upDirection);
}

// Read horizontal fov into camera
void FileReader::processHfov(std::istringstream& iss, Camera& camera) {
    double fov;
    if (!(iss >> fov)) {
        throw std::runtime_error("Error: Invalid or missing horizontal field of view.");
    }
    camera.setHorizontalFOV(fov);
}

// Read background color into sceneBuilder
void FileReader::processBackgroundColor(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    double r, g, b;
    if (!(iss >> r >> g >> b)) {
        throw std::runtime_error("Error: Invalid or missing background color values.");
    }
    Color backgroundColor(r, g, b);
    //backgroundColor.normalize();
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
    double odr, odg, odb, osr, osg, osb, ka, kd, ks, n, alpha, eta;
    if (!(iss >> odr >> odg >> odb >> osr >> osg >> osb >> ka >> kd >> ks >> n)) {
        throw std::runtime_error("Error: Invalid or missing material parameters.");
    }
    Color diffuse(odr, odg, odb);
    Color specular(osr, osg, osb);
    Material* material;
    if (iss >> alpha >> eta) {
        material = MaterialFactory::createReflectiveMaterial(diffuse, specular, ka, kd, ks, n, alpha, eta);
    }
    else {
        material = MaterialFactory::createMaterial(diffuse, specular, ka, kd, ks, n);
    }
    sceneBuilder.setCurrentMaterial(material);
}

// Read texture into currentTexture
void FileReader::processTexture(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    std::string filename;
    if (!(iss >> filename) || filename.empty()) {
        throw std::runtime_error("Error: Invalid or missing filename for texture.");
    }

    // Use shared_ptr for texture creation
    Texture* texture = new Texture(filename);
    sceneBuilder.setCurrentTexture(texture);
}

// Read normal map into currentNormalMap
void FileReader::processNormalMap(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    std::string filename;
    if (!(iss >> filename) || filename.empty()) {
        throw std::runtime_error("Error: Invalid or missing filename for normal map.");
    }

    // Use shared_ptr for normal map creation
    NormalMap* normalMap = new NormalMap(filename);
    sceneBuilder.setCurrentNormalMap(normalMap);
}


void FileReader::processSphere(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    double x, y, z, radius;
    AbstractShape* sphere;
    if (!(iss >> x >> y >> z >> radius)) {
        throw std::runtime_error("Error: Invalid sphere parameters.");
    }
    if (sceneBuilder.getCurrentMaterial() && sceneBuilder.getCurrentTexture() && sceneBuilder.getCurrentNormalMap()) {
        // create new normalMap, Texture, material
        Material* material = new Material(*sceneBuilder.getCurrentMaterial());
        sphere = SphereFactory::createNormalMappedSphere(Vec3(x, y, z), radius, *material, *sceneBuilder.getCurrentTexture(), *sceneBuilder.getCurrentNormalMap());
    }
    else if (sceneBuilder.getCurrentMaterial() && sceneBuilder.getCurrentTexture()) {
        // create new material and texture
        sphere = SphereFactory::createTexturedSphere(Vec3(x, y, z), radius, *sceneBuilder.getCurrentMaterial(), *sceneBuilder.getCurrentTexture());
    }
    else if (sceneBuilder.getCurrentMaterial()) {
        // create bew material
        Material* material = new Material(*sceneBuilder.getCurrentMaterial());
        sphere = SphereFactory::createMaterialSphere(Vec3(x, y, z), radius, *material);
    }
    else {
        sphere = SphereFactory::createSphere(Vec3(x, y, z), radius);
    }
    sceneBuilder.addShape(sphere);
}


// Read light into sceneBuilder
void FileReader::processLight(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    double x, y, z, r, g, b;
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

// Read attrubute light into sceneBuilder
void FileReader::processAttributeLight(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    double x, y, z, r, g, b, c1, c2, c3;
    int type;
    if (!(iss >> x >> y >> z >> type >> r >> g >> b >> c1 >> c2 >> c3)) {
        throw std::runtime_error("Error: Invalid or missing light parameters.");
    }
    Color color(r, g, b);
    Vec3 position(x, y, z);
    AbstractLight* light = nullptr;
    if (type == 1) {
        light = LightFactory::createAttributePointLight(position, color, c1, c2, c3);
    }
    else if (type == 0) {
        light = LightFactory::createAttributeDirectionalLight(position, color, c1, c2, c3);
    }
    else {
        throw std::runtime_error("Error: light type is not 0 or 1.");
    }
    sceneBuilder.addLight(light);
}

// Read vertex into verticies
void FileReader::processVertex(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    double x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing vertex coordinates.");
    }
    //Vec3* vertex = VertexFactory::createVertex(x, y, z);
    sceneBuilder.addVertex(Vec3(x, y, z));
}

// Read vertex normal into vertexNormals
void FileReader::processVertexNormal(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    double x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing vertex normal coordinates.");
    }
    //Vec3* vertexNormal = VertexFactory::createVertexNormal(x, y, z);
    sceneBuilder.addVertexNormal(Vec3(x, y, z).normal());
}

// Read texture coordinate into textureCoordinates
void FileReader::processTextureCoordinate(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    double u, v;
    if (!(iss >> u >> v)) {
        throw std::runtime_error("Error: Invalid or missing texture coordinates.");
    }
    //Vec2* textureCoordinate = VertexFactory::createTextureCoordinate(u, v);
    sceneBuilder.addTextureCoordinate(Vec2(u, v));
}

void FileReader::processTriangle(std::string& line, SceneBuilder& sceneBuilder) {
    int vertexAIndex, vertexBIndex, vertexCIndex;
    int textureCoordinateAIndex = -1, textureCoordinateBIndex = -1, textureCoordinateCIndex = -1;
    int vertexANormalIndex = -1, vertexBNormalIndex = -1, vertexCNormalIndex = -1;

    // Initialize attributes as null
    const Vec2* textureCoordinateA = nullptr;
    const Vec2* textureCoordinateB = nullptr;
    const Vec2* textureCoordinateC = nullptr;

    const Vec3* vertexANormal = nullptr;
    const Vec3* vertexBNormal = nullptr;
    const Vec3* vertexCNormal = nullptr;

    AbstractShape* triangle = nullptr;
    // Parse the input line
    if (sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertexAIndex, &textureCoordinateAIndex, &vertexANormalIndex, &vertexBIndex, &textureCoordinateBIndex, &vertexBNormalIndex, &vertexCIndex, &textureCoordinateCIndex, &vertexCNormalIndex) == 9) {
        // Full data with vertices, texture coordinates, and normals
        textureCoordinateA = &sceneBuilder.getTextureCoordinates()[textureCoordinateAIndex - 1];
        textureCoordinateB = &sceneBuilder.getTextureCoordinates()[textureCoordinateBIndex - 1];
        textureCoordinateC = &sceneBuilder.getTextureCoordinates()[textureCoordinateCIndex - 1];
        vertexANormal = &sceneBuilder.getVertexNormals()[vertexANormalIndex - 1];
        vertexBNormal = &sceneBuilder.getVertexNormals()[vertexBNormalIndex - 1];
        vertexCNormal = &sceneBuilder.getVertexNormals()[vertexCNormalIndex - 1];
    }
    else if (sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d", &vertexAIndex, &textureCoordinateAIndex, &vertexBIndex, &textureCoordinateBIndex, &vertexCIndex, &textureCoordinateCIndex) == 6) {
        // Vertices with texture coordinates only
        textureCoordinateA = &sceneBuilder.getTextureCoordinates()[textureCoordinateAIndex - 1];
        textureCoordinateB = &sceneBuilder.getTextureCoordinates()[textureCoordinateBIndex - 1];
        textureCoordinateC = &sceneBuilder.getTextureCoordinates()[textureCoordinateCIndex - 1];
    }
    else if (sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", &vertexAIndex, &vertexANormalIndex, &vertexBIndex, &vertexBNormalIndex, &vertexCIndex, &vertexCNormalIndex) == 6) {
        // Vertices with normals only
        vertexANormal = &sceneBuilder.getVertexNormals()[vertexANormalIndex - 1];
        vertexBNormal = &sceneBuilder.getVertexNormals()[vertexBNormalIndex - 1];
        vertexCNormal = &sceneBuilder.getVertexNormals()[vertexCNormalIndex - 1];
    }
    else if (sscanf_s(line.c_str(), "f %d %d %d", &vertexAIndex, &vertexBIndex, &vertexCIndex) == 3) { }
    else { throw std::runtime_error("Error: Triangle parameters are invalid."); }

    if (sceneBuilder.getCurrentMaterial() && sceneBuilder.getCurrentTexture() && sceneBuilder.getCurrentNormalMap()) {
        // create new normalMap, Texture, material
        Material* material = new Material(*sceneBuilder.getCurrentMaterial());
    }
    else if (sceneBuilder.getCurrentMaterial() && sceneBuilder.getCurrentTexture()) {
        // create new material and texture
    }
    else if (sceneBuilder.getCurrentMaterial()) {
        // create bew material
        Material* material = new Material(*sceneBuilder.getCurrentMaterial());
        triangle = TriangleFactory::createMaterialTriangle(sceneBuilder.getVertices()[vertexAIndex - 1], sceneBuilder.getVertices()[vertexBIndex - 1], sceneBuilder.getVertices()[vertexCIndex - 1], *material);
    }
    else {
        triangle = TriangleFactory::createTriangle(sceneBuilder.getVertices()[vertexAIndex - 1], sceneBuilder.getVertices()[vertexBIndex - 1], sceneBuilder.getVertices()[vertexCIndex - 1]);
    }
    sceneBuilder.addShape(triangle);
}

// Read depthCue into sceneBuilder
void FileReader::processDepthCue(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    double r, g, b, alphaMax, alphaMin, distanceMax, distanceMin;
    if (!(iss >> r >> g >> b >> alphaMax >> alphaMin >> distanceMax >> distanceMin)) {
        throw std::runtime_error("Error: Invalid or missing depthcueing parameters.");
    }
    Color depthCueColor(r, g, b);
    DepthCue* depthCue = new DepthCue(depthCueColor, alphaMax, alphaMin, distanceMax, distanceMin);
    sceneBuilder.setDepthCue(depthCue);
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
