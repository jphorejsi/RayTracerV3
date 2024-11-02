#include "fileManager.h"

#include "vecFactory.h"

#include <unordered_map>
#include <functional>


int FileReader::readFile(const std::string& filename, CameraBuilder& cameraBuilder, SceneBuilder& sceneBuilder, ImageSize& imageSize) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::unordered_map<std::string, bool> requiredParams = {
        {"eye", false}, {"viewdir", false}, {"updir", false},
        {"hfov", false}, {"bkgcolor", false}
    };

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

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
            processMaterial(iss);
        }
        else if (keyword == "texture") {
            //processTexture(iss, currentTexture);
        }
        else if (keyword == "sphere") {
            processSphere(iss, sceneBuilder);
        }
        else if (keyword == "light") {
            processLight(iss, sceneBuilder);
        }
        else if (keyword == "vertex") {
            processVertex(iss);
        }
        else if (keyword == "vertexNormal") {
            processVertexNormal(iss);
        }
        else if (keyword == "textureCoordinate") {
            processTextureCoordinate(iss);
        }
        else if (keyword == "f") {
            //processTriangle(line, sceneBuilder, currentMaterial, currentTexture, currentNormalMap);
        }
    }

    for (const auto& param : requiredParams) {
        if (!param.second) {
            throw std::runtime_error("Missing required parameter: " + param.first);
        }
    }

    return 0;
}

void FileReader::processEye(std::istringstream& iss, CameraBuilder& cameraBuilder) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing eye position coordinates.");
    }
    cameraBuilder.setEyePosition(Vec3(x, y, z));
}

void FileReader::processViewDir(std::istringstream& iss, CameraBuilder& cameraBuilder) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing view direction coordinates.");
    }
    cameraBuilder.setViewDirection(Vec3(x, y, z).normal());
}

void FileReader::processUpDir(std::istringstream& iss, CameraBuilder& cameraBuilder) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing up direction coordinates.");
    }
    cameraBuilder.setUpDirection(Vec3(x, y, z).normal());
}

void FileReader::processHfov(std::istringstream& iss, CameraBuilder& cameraBuilder) {
    float fov;
    if (!(iss >> fov)) {
        throw std::runtime_error("Error: Invalid or missing horizontal field of view.");
    }
    cameraBuilder.setHorizontalFOV(fov);
}

void FileReader::processBackgroundColor(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    float r, g, b;
    if (!(iss >> r >> g >> b)) {
        throw std::runtime_error("Error: Invalid or missing background color values.");
    }
    sceneBuilder.setBackgroundColor(Color(r, g, b));
}

void FileReader::processImageSize(std::istringstream& iss, ImageSize& imageSize) {
    int width, height;
    if (!(iss >> width >> height)) {
        throw std::runtime_error("Error: Invalid or missing image size.");
    }
    imageSize.setSize(width, height);
}

void FileReader::processMaterial(std::istringstream& iss) {
    float odr, odg, odb, osr, osg, osb, ka, kd, ks, n;
    if (!(iss >> odr >> odg >> odb >> osr >> osg >> osb >> ka >> kd >> ks >> n)) {
        throw std::runtime_error("Error: Invalid or missing material parameters.");
    }
    currentMaterial = MaterialFactory::createPhongMaterial(Color(odr, odg, odb), Color(osr, osg, osb), ka, kd, ks, n);
}

void FileReader::processTexture(std::istringstream& iss) {
    std::string filename;
    if (!(iss >> filename) || filename.empty()) {
        throw std::runtime_error("Error: Invalid or missing filename for texture.");
    }
    currentTexture = new Texture(filename);  // Assuming constructor handles file reading
}

void FileReader::processSphere(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    float x, y, z, r;
    if (!(iss >> x >> y >> z >> r)) {
        throw std::runtime_error("Error: Invalid or missing sphere parameters.");
    }
    Vec3 position(x, y, z);
    IShape* sphere = nullptr;
    if (currentMaterial && currentTexture && currentNormalMap) {
        sphere = SphereFactory::createNormalMappedShape(position, r, currentMaterial, currentTexture, currentNormalMap);
    }
    else if (currentMaterial && currentTexture) {
        sphere = SphereFactory::createTexturedShape(position, r, currentMaterial, currentTexture);
    }
    else if (currentMaterial) {
        sphere = SphereFactory::createShape(position, r, currentMaterial);
    }
    else {
        throw std::runtime_error("Error: Material is required to create a sphere.");
    }
    sceneBuilder.addShape(sphere);
}

void FileReader::processLight(std::istringstream& iss, SceneBuilder& sceneBuilder) {
    float x, y, z, r, g, b;
    int type;
    if (!(iss >> x >> y >> z >> type >> r >> g >> b)) {
        throw std::runtime_error("Error: Invalid or missing light parameters.");
    }
    Color color(r, g, b);
    Vec3 position(x, y, z);
    ILight* light = nullptr;
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

void FileReader::processVertex(std::istringstream& iss) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing vertex coordinates.");
    }
    Vec3* vertex = VertexFactory::createVertex(x, y, z);
    vertices.push_back(vertex);
}

void FileReader::processVertexNormal(std::istringstream& iss) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing vertex normal coordinates.");
    }
    Vec3* vertexNormal = VertexFactory::createVertexNormal(x, y, z);
    vertexNormals.push_back(vertexNormal);
}

void FileReader::processTextureCoordinate(std::istringstream& iss) {
    float u, v;
    if (!(iss >> u >> v)) {
        throw std::runtime_error("Error: Invalid or missing texture coordinates.");
    }
    Vec2* textureCoordinate = VertexFactory::createTextureCoordinate(u, v);
    textureCoordinates.push_back(textureCoordinate);
}

//void FileReader::processTriangle(std::string& line, SceneBuilder& sceneBuilder, IMaterial* currentMaterial, Texture* currentTexture, NormalMap* currentNormalMap) {
//    int indices[3];
//    if (sscanf(line.c_str(), "f %d %d %d", &indices[0], &indices[1], &indices[2]) == 3) {
//        Triangle* triangle = new Triangle(new Vec3(indices[0]), new Vec3(indices[1]), new Vec3(indices[2]));
//        triangle->setMaterial(currentMaterial);
//        triangle->setTexture(currentTexture);
//        triangle->setNormalMap(currentNormalMap);
//        sceneBuilder.addShape(triangle);
//    }
//}

std::string FileWriter::createPPMFile(const std::string& filename, const ImageSize& imageSize) {
    std::string ppmFilename = filename.substr(0, filename.find_last_of('.')) + ".ppm";
    std::ofstream outfile(ppmFilename);
    if (!outfile.is_open()) {
        throw std::runtime_error("Error: Unable to create PPM file: " + ppmFilename);
    }
    outfile << "P3\n" << imageSize.getWidth() << " " << imageSize.getHeight() << "\n255\n";
    return ppmFilename;
}
