#include "fileManager.h"
#include <unordered_map>
#include <functional>


// Write iss into camera
void FileReaderType::processEye(std::istringstream& iss, CameraType& camera) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing eye position coordinates.");
    }
    camera.setEyePosition(Vec3(x, y, z));
}

// Write view direction into camera
void FileReaderType::processViewDir(std::istringstream& iss, CameraType& camera) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing view direction coordinates.");
    }
    camera.setViewDirection((Vec3(x, y, z)).normal());
}

// Write up direction into camera
void FileReaderType::processUpDir(std::istringstream& iss, CameraType& camera) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing up direction coordinates.");
    }
    camera.setUpDirection((Vec3(x, y, z)).normal());
}

// Write horizontal fov into camera
void FileReaderType::processHfov(std::istringstream& iss, CameraType& camera) {
    float fov;
    if (!(iss >> fov)) {
        throw std::runtime_error("Error: Invalid or missing horizontal field of view.");
    }
    if (fov <= 0) {
        throw std::runtime_error("Error: Horizontal field of view must be positive.");
    }
    camera.setHorizontalFOV(fov);
}

// Write background color into scene
void FileReaderType::processBackgroundColor(std::istringstream& iss, SceneType& scene) {
    float r, g, b;
    if (!(iss >> r >> g >> b)) {
        throw std::runtime_error("Error: Invalid or missing background color components.");
    }
    if (r < 0 || g < 0 || b < 0 || r > 1 || g > 1 || b > 1) {
        throw std::runtime_error("Error: Background color values must be between 0 and 1.");
    }
    scene.setBackgroundColor(ColorType(r, g, b));
}

// Write iss into camera
void FileReaderType::processImageSize(std::istringstream& iss, ImageSizeType& imageSize) {
    int width, height;
    if (!(iss >> width >> height)) {
        throw std::runtime_error("Error: Invalid or missing image size.");
    }
    if (width <= 0 || height <= 0) {
        throw std::runtime_error("Error: Image dimensions must be positive.");
    }
    imageSize.setSize(width, height);
}


void FileReaderType::processMaterial(std::istringstream& iss, SceneType& scene) {
    float odr, odg, odb, osr, osg, osb, ka, kd, ks, n;
    if (!(iss >> odr >> odg >> odb >> osr >> osg >> osb >> ka >> kd >> ks >> n)) {
        throw std::runtime_error("Error: Invalid or missing material parameters.");
    }
    if (ka < 0 || kd < 0 || ks < 0 || n < 0) {
        throw std::runtime_error("Error: Material parameters must be non-negative.");
    }
    if (odr < 0 || odg < 0 || odb < 0 || odr > 1 || odg > 1 || odb > 1) {
        throw std::runtime_error("Error: Diffuse color values must be between 0 and 1.");
    }
    if (osr < 0 || osg < 0 || osb < 0 || osr > 1 || osg > 1 || osb > 1) {
        throw std::runtime_error("Error: Specular color values must be between 0 and 1.");
    }
    MaterialType* newMaterial = new MaterialType(ColorType(odr, odg, odb), ColorType(osr, osg, osb), ka, kd, ks, n);
    scene.addMaterial(newMaterial);
}


void FileReaderType::processSphere(std::istringstream& iss, SceneType& scene) {
    float x, y, z, r;
    if (!(iss >> x >> y >> z >> r)) {
        throw std::runtime_error("Error: Invalid or missing sphere parameters.");
    }
    if (r <= 0) {
        throw std::runtime_error("Error: Sphere radius must be positive.");
    }
    MaterialType* currentMaterial = scene.getMaterials().back();
    if (!currentMaterial) {
        throw std::runtime_error("Error: No material available for the sphere.");
    }
    TextureType* currentTexture = scene.getTextures().back();
    if (currentTexture) {
        TexturedSphereType* sphere = new TexturedSphereType(Vec3(x, y, z), r, currentMaterial, currentTexture);
        scene.addShape(sphere);
    }
    else {
        SphereType* sphere = new SphereType(Vec3(x, y, z), r, currentMaterial);
        scene.addShape(sphere);
    }
}


void FileReaderType::processLight(std::istringstream& iss, SceneType& scene) {
    float x, y, z, r, g, b;
    int type;
    if (!(iss >> x >> y >> z >> type >> r >> g >> b)) {
        throw std::runtime_error("Error: Invalid or missing light parameters.");
    }
    if (r < 0 || g < 0 || b < 0 || r > 1 || g > 1 || b > 1) {
        throw std::runtime_error("Error: Light color values must be between 0 and 1.");
    }
    if (type == 1) {
        scene.addLight(new PointLightType(Vec3(x, y, z), ColorType(r, g, b)));
    }
    else if (type == 0) {
        scene.addLight(new DirectionalLightType(Vec3(x, y, z), ColorType(r, g, b)));
    }
    else {
        throw std::runtime_error("Error: Invalid light type.");
    }
}


void FileReaderType::processAttLight(std::istringstream& iss, SceneType& scene) {
    float x, y, z, r, g, b, c1, c2, c3;
    int type;
    if (!(iss >> x >> y >> z >> type >> r >> g >> b >> c1 >> c2 >> c3)) {
        throw std::runtime_error("Error: Invalid or missing attribute light parameters.");
    }
    if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1) {
        throw std::runtime_error("Error: Attribute light color values must be between 0 and 1.");
    }
    if (type == 1) {
        AttributePointLightType* light = new AttributePointLightType(Vec3(x, y, z), ColorType(r, g, b), c1, c2, c3);
        scene.addLight(light);
    }
    else if (type == 0) {
        AttributeDirectionalLightType* light = new AttributeDirectionalLightType(Vec3(x, y, z), ColorType(r, g, b), c1, c2, c3);
        scene.addLight(light);
    }
    else {
        throw std::runtime_error("Error: Invalid attribute light type. Expected 0 or 1.");
    }
}


void FileReaderType::processTexture(std::istringstream& iss, SceneType& scene) {
    std::string filename;
    if (!(iss >> filename) || filename.empty()) {
        throw std::runtime_error("Error: Invalid or missing filename for texture.");
    }
    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".ppm") {
        throw std::runtime_error("Error: Invalid file format. Expected a .ppm file.");
    }

    std::ifstream textureFile(filename, std::ios::in | std::ios::binary);
    if (!textureFile.is_open()) {
        throw std::runtime_error("Error: Unable to open texture file: " + filename);
    }

    std::string name;
    int width, height, maxValue;
    if (!(textureFile >> name >> width >> height >> maxValue)) {
        throw std::runtime_error("Error: Failed to read texture dimensions or max value.");
    }
    if (width <= 0 || height <= 0) {
        throw std::runtime_error("Error: Texture dimensions must be greater than zero.");
    }

    TextureType* texture = new TextureType(width, height, maxValue);
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            int r, g, b;
            if (!(textureFile >> r >> g >> b)) {
                throw std::runtime_error("Error: Failed to read pixel data from texture file.");
            }
            ColorType color(
                r / static_cast<float>(maxValue),
                g / static_cast<float>(maxValue),
                b / static_cast<float>(maxValue)
            );
            texture->setPixel(i, j, color);
        }
    }
    scene.addTexture(texture);
}


void FileReaderType::processVertex(std::istringstream& iss, SceneType& scene) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing vertex coordinates.");
    }
    scene.addVertex(new Vec3(x, y, z));
}


void FileReaderType::processNormalVector(std::istringstream& iss, SceneType& scene) {
    float x, y, z;
    if (!(iss >> x >> y >> z)) {
        throw std::runtime_error("Error: Invalid or missing vertex normal coordinates.");
    }
    scene.addNormalVector(new Vec3(x, y, z));
}


void FileReaderType::processTextureCoordinate(std::istringstream& iss, SceneType& scene) {
    float u, v;
    if (!(iss >> u >> v)) {
        throw std::runtime_error("Error: Invalid or missing texture coordinates.");
    }
    scene.addTextureCoordinate(new Vec2(u, v));
}


void FileReaderType::processFace(const std::string& line, SceneType& scene) {
    int int_var[10];  // For holding vertex and normal indices
    TriangleType* triangle = nullptr;
    if (!currentMaterial) {
        throw std::runtime_error("Error: No material available for the face.");
    }
    if (sscanf_s(line.c_str(), "f %d %d %d", int_var, int_var + 1, int_var + 2) == 3) {
        triangle = new TriangleType(vertices[int_var[0] - 1], vertices[int_var[1] - 1], vertices[int_var[2] - 1], currentMaterial);
    }
    else if (sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", int_var, int_var + 1, int_var + 2, int_var + 3, int_var + 4, int_var + 5) == 6) {
        triangle = new SmoothShadedTriangleType(
            vertices[int_var[0] - 1], vertices[int_var[1] - 1], vertices[int_var[2] - 1], currentMaterial, normalVectors[int_var[3] - 1], normalVectors[int_var[4] - 1], normalVectors[int_var[5] - 1]);
    }
    else {
        throw std::runtime_error("Error: Invalid face data format.");
    }
    scene.addShape(triangle);
}


int FileReaderType::readFile(const std::string& filename, CameraType& camera, SceneType& scene, ImageSizeType& imageSize) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::unordered_map<std::string, bool> requiredParams = {
        {"eye", false}, {"viewdir", false}, {"updir", false},
        {"hfov", false}, {"bkgcolor", false}
    };

    // Define the keyword handlers
    std::unordered_map<std::string, std::function<void(std::istringstream&)>> keywordHandlers = {
        {"eye", [&](std::istringstream& iss) {
            this->processEye(iss, camera);
            requiredParams["eye"] = true;
        }},
        {"viewdir", [&](std::istringstream& iss) {
            this->processViewDir(iss, camera);
            requiredParams["viewdir"] = true;
        }},
        {"updir", [&](std::istringstream& iss) {
            this->processUpDir(iss, camera);
            requiredParams["updir"] = true;
        }},
        {"hfov", [&](std::istringstream& iss) {
            this->processHfov(iss, camera);
            requiredParams["hfov"] = true;
        }},
        {"bkgcolor", [&](std::istringstream& iss) {
            this->processBackgroundColor(iss, scene);
            requiredParams["bkgcolor"] = true;
        }},
        {"imsize", [&](std::istringstream& iss) { this->processImageSize(iss, imageSize); }},
        {"mtlcolor", [&](std::istringstream& iss) { this->processMaterial(iss, scene); }},
        {"sphere", [&](std::istringstream& iss) { this->processSphere(iss, scene.getMaterials().back(), scene.getTextures().back(), scene); }},
        {"light", [&](std::istringstream& iss) { this->processLight(iss, scene); }},
        {"attlight", [&](std::istringstream& iss) { this->processAttLight(iss, scene); }},
        {"texture", [&](std::istringstream& iss) { this->processTexture(iss, scene); }},
        {"v", [&](std::istringstream& iss) { this->processVertex(iss, scene); }},
        {"vn", [&](std::istringstream& iss) { this->processVertexNormal(iss, normalVectors); }},
        {"vt", [&](std::istringstream& iss) { this->processTextureCoordinate(iss, textureCoordinates); }}
    };

    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        if (keywordHandlers.find(keyword) != keywordHandlers.end()) {
            keywordHandlers[keyword](iss);
        }
        else if (keyword == "f") {
            this->processFace(line, scene.getMaterials().back(), scene.getTextures().back(), scene.getVertices(), scene.getNormalVectors(), scene.getTextureCoordinates(), scene);
        }
        else if (keyword == "") {
            continue;
        }
        else {
            throw std::runtime_error("Unknown keyword: " + keyword);
        }
    }

    // Check if all required parameters were provided
    for (const auto& param : requiredParams) {
        const std::string& key = param.first;
        bool provided = param.second;

        if (!provided) {
            throw std::runtime_error("Missing required parameter: " + key);
        }
    }

    // Clean up dynamic memory allocations
    //for (Vec3* vertex : vertices) delete vertex;
    //for (Vec3* normal : normalVectors) delete normal;
    //for (Vec2* texCoord : textureCoordinates) delete texCoord;

    return 0;
}

std::string FileWriterType::createPPMFile(std::string filename, const ImageSizeType& imageSize) {
    // Change file extension to .ppm
    filename.pop_back(); filename.pop_back(); filename.pop_back();
    filename.append("ppm");

    // Open output file stream
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        throw std::runtime_error("Error: Unable to create PPM file: " + filename);
    }

    // Write PPM file header
    outfile << "P3" << "\n" << imageSize.getWidth() << " " << imageSize.getHeight() << "\n" << "255\n";
    outfile.close();

    return filename;
}

//void FileWriterType::RenderToPPM(const std::string& filename, const ImageSizeType& imageSize, const ViewFrustrumType& viewingWindow, const CameraType& camera, const SceneType& scene) {
//    // Open the output file stream
//    std::ofstream outfile(filename);
//    if (!outfile.is_open()) {
//        throw std::runtime_error("Error: Unable to create PPM file: " + filename);
//    }
//
//    // Write the PPM file header
//    outfile << "P3" << "\n" << imageSize.getWidth() << " " << imageSize.getHeight() << "\n" << "255\n";
//
//    // Calculate horizontal and vertical step sizes based on the image dimensions
//    Vec3 deltaH = (viewingWindow.getUpperRight() - viewingWindow.getUpperLeft()) / (float)(imageSize.getWidth() - 1);
//    Vec3 deltaV = (viewingWindow.getLowerLeft() - viewingWindow.getUpperLeft()) / (float)(imageSize.getHeight() - 1);
//
//    Vec3 point;
//
//    // Loop through each pixel in the image
//    for (int j = 0; j < imageSize.getHeight(); j++) {
//        for (int i = 0; i < imageSize.getWidth(); i++) {
//            // Calculate the current point in the viewing window
//            point = viewingWindow.getUpperLeft() + deltaH * (float)i + deltaV * (float)j;
//
//            // Create a ray from the camera's eye position towards the point
//            Ray ray;
//            ray.setOrigin(camera.getEyePosition());
//            ray.setDirection((point - camera.getEyePosition()).normal());
//
//            // Trace the ray to find the color at the current pixel
//            ColorType color = ray.traceRay(scene);
//
//            // Convert the color values from float [0, 1] to int [0, 255] and write to the file
//            int r = static_cast<int>(color.getR() * 255.0f);
//            int g = static_cast<int>(color.getG() * 255.0f);
//            int b = static_cast<int>(color.getB() * 255.0f);
//            outfile << r << " " << g << " " << b << "\n";
//        }
//    }
//
//    // Close the output file stream
//    outfile.close();
//}


