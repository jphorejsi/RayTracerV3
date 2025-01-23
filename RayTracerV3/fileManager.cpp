#include "fileManager.h"
#include <unordered_map>
//#include <stdexcept>


void FileReader::readFile(const std::string& filename, Camera& camera, Scene& scene, Renderer& system) {
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
            processBackgroundColor(iss, scene);
            requiredParams["bkgcolor"] = true;
        }
        else if (keyword == "imsize") {
            processImageSize(iss, system);
        }
        else if (keyword == "mtlcolor") {
            processMaterial(iss);
        }
        else if (keyword == "texture") {
            processTexture(iss);
        }
        else if (keyword == "sphere") {
            processSphere(iss, scene);
        }
        else if (keyword == "light") {
            processLight(iss, scene);
        }
        else if (keyword == "attlight") {
            processAttributeLight(iss, scene);
        }
        else if (keyword == "v") {
            processVertex(iss);
        }
        else if (keyword == "vn") {
            processVertexNormal(iss);
        }
        else if (keyword == "vt") {
            processTextureCoordinate(iss);
        }
        else if (keyword == "f") {
            processTriangle(line, scene);
        }
        else if (keyword == "depthcueing") {
            processDepthCue(iss, scene);
        }
        else if (keyword == "normalmap") {
            processNormalMap(iss);
        }
    }
    // Check required parameters were provided
    for (const auto& param : requiredParams) {
        if (!param.second) {
            throw std::runtime_error("Missing required parameter: " + param.first);
        }
    }
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
void FileReader::processBackgroundColor(std::istringstream& iss, Scene& scene) {
    double r, g, b;
    if (!(iss >> r >> g >> b)) {
        throw std::runtime_error("Error: Invalid or missing background color values.");
    }
    Color backgroundColor(r, g, b);
    scene.setBackgroundColor(backgroundColor);
}

// Read image size into imageSize
void FileReader::processImageSize(std::istringstream& iss, Renderer& system) {
    int width, height;
    if (!(iss >> width >> height)) {
        throw std::runtime_error("Error: Invalid or missing image size.");
    }
    system.setSize(width, height);
}

// Read material into sceneBuilder
void FileReader::processMaterial(std::istringstream& iss) {
    double odr, odg, odb, osr, osg, osb, ka, kd, ks, n, alpha, eta;

    // Parse material parameters
    if (!(iss >> odr >> odg >> odb >> osr >> osg >> osb >> ka >> kd >> ks >> n)) {
        throw std::runtime_error("Error: Invalid or missing material parameters.");
    }

    // Create diffuse and specular colors
    Color diffuse(odr, odg, odb);
    Color specular(osr, osg, osb);

    // Create a material shared pointer
    if (iss >> alpha >> eta) {
        this->currentMaterial = std::make_shared<ReflectiveBlinnPhong>(diffuse, specular, ka, kd, ks, n, alpha, eta);
    }   
    else {
        this->currentMaterial = std::make_shared<BlinnPhong>(diffuse, specular, ka, kd, ks, n);
    }
}

// Read texture into currentTexture
void FileReader::processTexture(std::istringstream& iss) {
    std::string filename;
    if (!(iss >> filename) || filename.empty()) {
        throw std::runtime_error("Error: Invalid or missing filename for texture.");
    }

    // Use shared_ptr for texture creation
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(filename);
    this->currentMaterial->setTexture(texture);
}

// Read normal map into currentNormalMap
void FileReader::processNormalMap(std::istringstream& iss) {
    std::string filename;
    if (!(iss >> filename) || filename.empty()) {
        throw std::runtime_error("Error: Invalid or missing filename for texture.");
    }

    // Use shared_ptr for texture creation
    std::shared_ptr<NormalMap> normalMap = std::make_shared<NormalMap>(filename);
    currentMaterial->setNormalMap(normalMap);
}


void FileReader::processSphere(std::istringstream& iss, Scene& scene) {
    double x, y, z, radius;
    Shape* sphere;
    if (!(iss >> x >> y >> z >> radius)) {
        throw std::runtime_error("Error: Invalid sphere parameters.");
    }
    if (currentMaterial) {
        sphere = new Sphere(Vec3(x, y, z), radius, this->currentMaterial);
    }
    else {
        throw std::runtime_error("Material must be defined before adding shape");
    }
    scene.addShape(sphere);
}

// Read light into sceneBuilder
void FileReader::processLight(std::istringstream& iss, Scene& scene) {
    double x, y, z, r, g, b;
    int type;
    if (!(iss >> x >> y >> z >> type >> r >> g >> b)) {
        throw std::runtime_error("Error: Invalid or missing light parameters.");
    }
    Color color(r, g, b);
    Vec3 position(x, y, z);
    AbstractLight* light = nullptr;
    if (type == 1) {
        light = new PointLight(position, color);
    }
    else if (type == 0) {
        light = new DirectionalLight(position, color);
    }
    else {
        throw std::runtime_error("Error: light type is not 0 or 1.");
    }
    scene.addLight(light);
}

// Read attrubute light into sceneBuilder
void FileReader::processAttributeLight(std::istringstream& iss, Scene& scene) {
    double x, y, z, r, g, b, c1, c2, c3;
    int type;
    if (!(iss >> x >> y >> z >> type >> r >> g >> b >> c1 >> c2 >> c3)) {
        throw std::runtime_error("Error: Invalid or missing light parameters.");
    }
    Color color(r, g, b);
    Vec3 position(x, y, z);
    AbstractLight* light = nullptr;
    if (type == 1) {
        light = new AttributePointLight(position, color, c1, c2, c3);
    }
    else if (type == 0) {
        light = new AttributeDirectionalLight(position, color, c1, c2, c3);
    }
    else {
        throw std::runtime_error("Error: light type is not 0 or 1.");
    }
    scene.addLight(light);
}

// Read vertex into verticies
void FileReader::processVertex(std::istringstream& iss) {
    double x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing vertex coordinates.");
    }
    this->vertices.push_back(Vec3(x, y, z));
}

// Read vertex normal into vertexNormals
void FileReader::processVertexNormal(std::istringstream& iss) {
    double x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing vertex normal coordinates.");
    }
    this->VertexNormals.push_back(Vec3(x, y, z).normal());
}

// Read texture coordinate into textureCoordinates
void FileReader::processTextureCoordinate(std::istringstream& iss) {
    double u, v;
    if (!(iss >> u >> v)) {
        throw std::runtime_error("Error: Invalid or missing texture coordinates.");
    }
    if (u > 1 || u < 0 || v > 1 || v < 0) {
        throw std::runtime_error("Error: Texture coordinate outside range 0 - 1.");
    }
    this->textureCoordinates.push_back(Vec2(u, v));
}


void FileReader::processTriangle(std::string& line, Scene& scene) { // TODO
    int vertexAIndex, vertexBIndex, vertexCIndex;
    int textureCoordinateAIndex = -1, textureCoordinateBIndex = -1, textureCoordinateCIndex = -1;
    int vertexANormalIndex = -1, vertexBNormalIndex = -1, vertexCNormalIndex = -1;

    // Initialize attributes as null
    Vec2* textureCoordinateA = nullptr;
    Vec2* textureCoordinateB = nullptr;
    Vec2* textureCoordinateC = nullptr;

    Vec3* vertexANormal = nullptr;
    Vec3* vertexBNormal = nullptr;
    Vec3* vertexCNormal = nullptr;

    Triangle* triangle = nullptr;
    // Parse the input line
    if (sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertexAIndex, &textureCoordinateAIndex, &vertexANormalIndex, &vertexBIndex, &textureCoordinateBIndex, &vertexBNormalIndex, &vertexCIndex, &textureCoordinateCIndex, &vertexCNormalIndex) == 9) {
        // Full data with vertices, texture coordinates, and normals
        textureCoordinateA = &this->textureCoordinates[textureCoordinateAIndex - 1];
        textureCoordinateB = &this->textureCoordinates[textureCoordinateBIndex - 1];
        textureCoordinateC = &this->textureCoordinates[textureCoordinateCIndex - 1];
        vertexANormal = &this->VertexNormals[vertexANormalIndex - 1];
        vertexBNormal = &this->VertexNormals[vertexBNormalIndex - 1];
        vertexCNormal = &this->VertexNormals[vertexCNormalIndex - 1];
    }
    else if (sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d", &vertexAIndex, &textureCoordinateAIndex, &vertexBIndex, &textureCoordinateBIndex, &vertexCIndex, &textureCoordinateCIndex) == 6) {
        // Vertices with texture coordinates only
        textureCoordinateA = &this->textureCoordinates[textureCoordinateAIndex - 1];
        textureCoordinateB = &this->textureCoordinates[textureCoordinateBIndex - 1];
        textureCoordinateC = &this->textureCoordinates[textureCoordinateCIndex - 1];
    }
    else if (sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", &vertexAIndex, &vertexANormalIndex, &vertexBIndex, &vertexBNormalIndex, &vertexCIndex, &vertexCNormalIndex) == 6) {
        // Vertices with normals only
        vertexANormal = &this->VertexNormals[vertexANormalIndex - 1];
        vertexBNormal = &this->VertexNormals[vertexBNormalIndex - 1];
        vertexCNormal = &this->VertexNormals[vertexCNormalIndex - 1];
    }
    else if (sscanf_s(line.c_str(), "f %d %d %d", &vertexAIndex, &vertexBIndex, &vertexCIndex) == 3) {
    }
    else { 
        throw std::runtime_error("Error: Triangle parameters are invalid.");
    }

    if (currentMaterial) {
        triangle = new Triangle(this->vertices[vertexAIndex - 1], this->vertices[vertexBIndex - 1], this->vertices[vertexCIndex - 1], currentMaterial);
        triangle->setTextureCoordinateA(textureCoordinateA);
        triangle->setTextureCoordinateB(textureCoordinateB);
        triangle->setTextureCoordinateC(textureCoordinateC);
        triangle->setVertexANormal(vertexANormal);
        triangle->setVertexBNormal(vertexBNormal);
        triangle->setVertexCNormal(vertexCNormal);
    }
    else {
        throw std::runtime_error("Material must be defined before adding shape");
    }
    scene.addShape(triangle);
}

// Read depthCue into sceneBuilder
void FileReader::processDepthCue(std::istringstream& iss, Scene& scene) {
    double r, g, b, alphaMax, alphaMin, distanceMax, distanceMin;
    if (!(iss >> r >> g >> b >> alphaMax >> alphaMin >> distanceMax >> distanceMin)) {
        throw std::runtime_error("Error: Invalid or missing depthcueing parameters.");
    }
    Color depthCueColor(r, g, b);
    DepthCue* depthCue = new DepthCue(depthCueColor, alphaMax, alphaMin, distanceMax, distanceMin);
    scene.setDepthCue(depthCue);
}