//#include "fileManager.h"
//#include <unordered_map>
//#include <functional>
//
//// Process functions using CameraBuilder
//
//// Read eye position into CameraBuilder
//void FileReader::processEye(std::istringstream& iss, CameraBuilder& cameraBuilder) {
//    float x, y, z;
//    if (!(iss >> x >> y >> z)) {
//        throw std::runtime_error("Error: Invalid or missing eye position coordinates.");
//    }
//    cameraBuilder.setEyePosition(Vec3(x, y, z));
//}
//
//// Read view direction into CameraBuilder
//void FileReader::processViewDir(std::istringstream& iss, CameraBuilder& cameraBuilder) {
//    float x, y, z;
//    if (!(iss >> x >> y >> z)) {
//        throw std::runtime_error("Error: Invalid or missing view direction coordinates.");
//    }
//    cameraBuilder.setViewDirection((Vec3(x, y, z)).normal());
//}
//
//// Read up direction into CameraBuilder
//void FileReader::processUpDir(std::istringstream& iss, CameraBuilder& cameraBuilder) {
//    float x, y, z;
//    if (!(iss >> x >> y >> z)) {
//        throw std::runtime_error("Error: Invalid or missing up direction coordinates.");
//    }
//    cameraBuilder.setUpDirection((Vec3(x, y, z)).normal());
//}
//
//// Read horizontal field of view into CameraBuilder
//void FileReader::processHfov(std::istringstream& iss, CameraBuilder& cameraBuilder) {
//    float fov;
//    if (!(iss >> fov)) {
//        throw std::runtime_error("Error: Invalid or missing horizontal field of view.");
//    }
//    if (fov <= 0) {
//        throw std::runtime_error("Error: Horizontal field of view must be positive.");
//    }
//    cameraBuilder.setHorizontalFOV(fov);
//}
//
//// Process functions using SceneBuilder
//
//// Read background color into SceneBuilder
//void FileReader::processBackgroundColor(std::istringstream& iss, SceneBuilder& sceneBuilder) {
//    float r, g, b;
//    if (!(iss >> r >> g >> b)) {
//        throw std::runtime_error("Error: Invalid or missing background color components.");
//    }
//    if (r < 0 || g < 0 || b < 0 || r > 1 || g > 1 || b > 1) {
//        throw std::runtime_error("Error: Background color values must be between 0 and 1.");
//    }
//    sceneBuilder.setBackgroundColor(Color(r, g, b));
//}
//
//// read imagesize parameters into imageSize
//void FileReader::processImageSize(std::istringstream& iss, ImageSize& imageSize) {
//    int width, height;
//    if (!(iss >> width >> height)) {
//        throw std::runtime_error("Error: Invalid or missing image size.");
//    }
//    if (width <= 0 || height <= 0) {
//        throw std::runtime_error("Error: Image dimensions must be positive.");
//    }
//    imageSize.setSize(width, height);
//}
//
//void FileReader::processMaterial(std::istringstream& iss, SceneBuilder& sceneBuilder) {
//    float odr, odg, odb, osr, osg, osb, ka, kd, ks, n;
//    if (!(iss >> odr >> odg >> odb >> osr >> osg >> osb >> ka >> kd >> ks >> n)) {
//        throw std::runtime_error("Error: Invalid or missing material parameters.");
//    }
//    Color od(odr, odg, odb);
//    Color os(osr, osg, osb);
//    IMaterial* material = MaterialFactory::createPhongMaterial(od, os, ka, kd, ks, n);
//    sceneBuilder.addMaterial(material);
//}
//
//// Read sphere parameters into SceneBuilder
//void FileReader::processSphere(std::istringstream& iss, SceneBuilder& sceneBuilder) {
//    float x, y, z, r;
//    if (!(iss >> x >> y >> z >> r)) {
//        throw std::runtime_error("Error: Invalid or missing sphere parameters.");
//    }
//    Vec3 position(x, y, z);
//    IMaterial* currentMaterial = sceneBuilder.getMaterials().empty() ? nullptr : sceneBuilder.getMaterials().back();
//    Texture* currentTexture = sceneBuilder.getTextures().empty() ? nullptr : sceneBuilder.getTextures().back();
//    NormalMap* currentNormalMap = sceneBuilder.getNormalMaps().empty() ? nullptr : sceneBuilder.getNormalMaps().back();
//    IShape* sphere;
//    if (currentNormalMap) {
//        sphere = SphereFactory::createNormalMappedShape(position, r, currentMaterial, currentTexture, currentNormalMap);
//
//    }
//    else if (currentTexture) {
//        sphere = SphereFactory::createTexturedShape(position, r, currentMaterial, currentTexture);
//    }
//    else {
//        sphere = SphereFactory::createShape(position, r, currentMaterial);
//    }
//    sceneBuilder.addShape(sphere);
//}
//
//
//void FileReader::processLight(std::istringstream& iss, SceneBuilder& sceneBuilder) {
//    float x, y, z, r, g, b;
//    int type;
//    if (!(iss >> x >> y >> z >> type >> r >> g >> b)) {
//        throw std::runtime_error("Error: Invalid or missing light parameters.");
//    }
//    Color color(r, g, b);
//    Vec3 position(x, y, z);
//    ILight* light = nullptr;
//    if (type == 1) {
//        light = LightFactory::createPointLight(position, color);
//    }
//    else if (type == 0) {
//        light = LightFactory::createDirectionalLight(position, color);
//    }
//    else {
//        throw std::runtime_error("Error: Invalid light type. Expected 0 (Directional) or 1 (Point).");
//    }
//    sceneBuilder.addLight(light);
//}
//
//// Read attribute light parameters into SceneBuilder
//void FileReader::processAttLight(std::istringstream& iss, SceneBuilder& sceneBuilder) {
//    float x, y, z, r, g, b, c1, c2, c3;
//    int type;
//    if (!(iss >> x >> y >> z >> type >> r >> g >> b >> c1 >> c2 >> c3)) {
//        throw std::runtime_error("Error: Invalid or missing attribute light parameters.");
//    }
//    if (r < 0 || r > 1 || g < 0 || g > 1 || b < 0 || b > 1) {
//        throw std::runtime_error("Error: Attribute light color values must be between 0 and 1.");
//    }
//    if (type == 1) {
//        sceneBuilder.addLight(new AttributePointLight(Vec3(x, y, z), Color(r, g, b), c1, c2, c3));
//    }
//    else if (type == 0) {
//        sceneBuilder.addLight(new AttributeDirectionalLight(Vec3(x, y, z), Color(r, g, b), c1, c2, c3));
//    }
//    else {
//        throw std::runtime_error("Error: Invalid attribute light type. Expected 0 or 1.");
//    }
//}
//
//// Read texture information into SceneBuilder
//void FileReader::processTexture(std::istringstream& iss, SceneBuilder& sceneBuilder) {
//    std::string filename;
//    if (!(iss >> filename) || filename.empty()) {
//        throw std::runtime_error("Error: Invalid or missing filename for texture.");
//    }
//    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".ppm") {
//        throw std::runtime_error("Error: Invalid file format. Expected a .ppm file.");
//    }
//
//    std::ifstream textureFile(filename, std::ios::in | std::ios::binary);
//    if (!textureFile.is_open()) {
//        throw std::runtime_error("Error: Unable to open texture file: " + filename);
//    }
//
//    std::string header;
//    int width, height, maxValue;
//
//    // Read PPM file header
//    textureFile >> header >> width >> height >> maxValue;
//
//    if (header != "P3" || width <= 0 || height <= 0 || maxValue != 255) {
//        throw std::runtime_error("Error: Invalid PPM file header.");
//    }
//
//    // Skip the rest of the line after the header
//    textureFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//
//    // Create a new Texture object
//    Texture* texture = new Texture(width, height, maxValue);
//
//    // Read pixel data
//    for (int j = 0; j < height; ++j) {
//        for (int i = 0; i < width; ++i) {
//            int r, g, b;
//            if (!(textureFile >> r >> g >> b)) {
//                delete texture;
//                throw std::runtime_error("Error: Failed to read pixel data from texture file.");
//            }
//            // Normalize pixel values and set them in the texture
//            Color color(
//                r / static_cast<float>(maxValue),
//                g / static_cast<float>(maxValue),
//                b / static_cast<float>(maxValue)
//            );
//            texture->setPixel(i, j, color);
//        }
//    }
//
//    // Add the texture to the scene using the SceneBuilder
//    sceneBuilder.addTexture(texture);
//}
//
//// Read vertex into SceneBuilder
//void FileReader::processVertex(std::istringstream& iss, SceneBuilder& sceneBuilder) {
//    float x, y, z;
//    if (!(iss >> x >> y >> z)) {
//        throw std::runtime_error("Error: Invalid or missing vertex coordinates.");
//    }
//    sceneBuilder.addVertex(new Vec3(x, y, z));
//}
//
//// Read vertex normal into SceneBuilder
//void FileReader::processVertexNormal(std::istringstream& iss, SceneBuilder& sceneBuilder) {
//    float x, y, z;
//    if (!(iss >> x >> y >> z)) {
//        throw std::runtime_error("Error: Invalid or missing vertex normal coordinates.");
//    }
//    sceneBuilder.addVertexNormal(new Vec3(x, y, z));
//}
//
//// Read texture coordinate into SceneBuilder
//void FileReader::processTextureCoordinate(std::istringstream& iss, SceneBuilder& sceneBuilder) {
//    float u, v;
//    if (!(iss >> u >> v)) {
//        throw std::runtime_error("Error: Invalid or missing texture coordinates.");
//    }
//    sceneBuilder.addTextureCoordinate(new Vec2(u, v));
//}
//
//// Read triangle face into SceneBuilder
//void FileReader::processTriangle(std::string& line, SceneBuilder& sceneBuilder) {
//    int int_var[9] = { 0 };
//
//    // Retrieve the most recent material, texture, and normal map from SceneBuilder
//    IMaterial* currentMaterial = sceneBuilder.getMaterials().empty() ? nullptr : sceneBuilder.getMaterials().back();
//    Texture* currentTexture = sceneBuilder.getTextures().empty() ? nullptr : sceneBuilder.getTextures().back();
//    NormalMap* currentNormalMap = sceneBuilder.getNormalMaps().empty() ? nullptr : sceneBuilder.getNormalMaps().back();
//
//    TriangleFactory triangleFactory;
//    IShape* triangle = nullptr;
//
//    if (sscanf_s(line.c_str(), "f %d %d %d", int_var, int_var + 1, int_var + 2) == 3) {
//        triangle = triangleFactory.createShape(new Vec3(int_var[0]), new Vec3(int_var[1]), new Vec3(int_var[2]), currentMaterial);
//    }
//    //else if (sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", int_var, int_var + 1, int_var + 2, int_var + 3, int_var + 4, int_var + 5) == 6) {
//    //    triangle = triangleFactory.createNormalMappedShape(new Vec3(int_var[0]), new Vec3(int_var[2]), new Vec3(int_var[4]), currentMaterial, currentTexture, currentNormalMap);
//    //}
//    //else if (sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d", int_var, int_var + 1, int_var + 2, int_var + 3, int_var + 4, int_var + 5) == 6) {
//    //    triangle = triangleFactory.createTexturedShape(new Vec3(int_var[0]), new Vec3(int_var[2]), new Vec3(int_var[4]), currentMaterial, currentTexture);
//    //}
//    //else if (sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", int_var, int_var + 1, int_var + 2, int_var + 3, int_var + 4, int_var + 5, int_var + 6, int_var + 7, int_var + 8) == 9) {
//    //    triangle = triangleFactory.createNormalMappedShape(new Vec3(int_var[0]), new Vec3(int_var[3]), new Vec3(int_var[6]), currentMaterial, currentTexture, currentNormalMap);
//    //}
//
//    if (triangle) {
//        sceneBuilder.addShape(triangle);
//    }
//    else {
//        throw std::runtime_error("Error: Invalid triangle parameters.");
//    }
//}
//
//
//int FileReader::readFile(const std::string& filename, CameraBuilder& cameraBuilder, SceneBuilder& sceneBuilder, ImageSize& imageSize) {
//    std::ifstream inputFile(filename);
//    if (!inputFile.is_open()) {
//        throw std::runtime_error("Unable to open file: " + filename);
//    }
//
//    std::unordered_map<std::string, bool> requiredParams = {
//        {"eye", false}, {"viewdir", false}, {"updir", false},
//        {"hfov", false}, {"bkgcolor", false}
//    };
//
//    std::unordered_map<std::string, std::function<void(std::istringstream&)>> keywordHandlers = {
//        {"eye", [&](std::istringstream& iss) {
//            FileReader::processEye(iss, cameraBuilder);
//            requiredParams["eye"] = true;
//        }},
//        {"viewdir", [&](std::istringstream& iss) {
//            FileReader::processViewDir(iss, cameraBuilder);
//            requiredParams["viewdir"] = true;
//        }},
//        {"updir", [&](std::istringstream& iss) {
//            FileReader::processUpDir(iss, cameraBuilder);
//            requiredParams["updir"] = true;
//        }},
//        {"hfov", [&](std::istringstream& iss) {
//            FileReader::processHfov(iss, cameraBuilder);
//            requiredParams["hfov"] = true;
//        }},
//        {"bkgcolor", [&](std::istringstream& iss) {
//            FileReader::processBackgroundColor(iss, sceneBuilder);
//            requiredParams["bkgcolor"] = true;
//        }},
//        {"imsize", [&](std::istringstream& iss) {
//            FileReader::processImageSize(iss, imageSize);
//        }},
//        {"mtlcolor", [&](std::istringstream& iss) {
//            FileReader::processMaterial(iss, sceneBuilder);
//        }},
//        {"sphere", [&](std::istringstream& iss) {
//            FileReader::processSphere(iss, sceneBuilder);
//        }},
//        {"light", [&](std::istringstream& iss) {
//            FileReader::processLight(iss, sceneBuilder);
//        }},
//        {"attlight", [&](std::istringstream& iss) {
//            FileReader::processAttLight(iss, sceneBuilder);
//        }},
//        {"texture", [&](std::istringstream& iss) {
//            FileReader::processTexture(iss, sceneBuilder);
//        }},
//        {"v", [&](std::istringstream& iss) {
//            FileReader::processVertex(iss, sceneBuilder);
//        }},
//        {"vn", [&](std::istringstream& iss) {
//            FileReader::processVertexNormal(iss, sceneBuilder);
//        }},
//        {"vt", [&](std::istringstream& iss) {
//            FileReader::processTextureCoordinate(iss, sceneBuilder);
//        }}
//    };
//
//    std::string line;
//    while (std::getline(inputFile, line)) {
//        std::istringstream iss(line);
//        std::string keyword;
//        iss >> keyword;
//
//        if (keywordHandlers.find(keyword) != keywordHandlers.end()) {
//            keywordHandlers[keyword](iss);
//        }
//        else if (keyword == "f") {
//            FileReader::processTriangle(line, sceneBuilder);
//        }
//        else if (keyword == "") {
//            continue;
//        }
//        else {
//            throw std::runtime_error("Unknown keyword: " + keyword);
//        }
//    }
//
//    for (const auto& param : requiredParams) {
//        const std::string& key = param.first;
//        bool provided = param.second;
//        if (!provided) {
//            throw std::runtime_error("Missing required parameter: " + key);
//        }
//    }
//
//    return 0;
//}
//
//
//// Example function to create a PPM file based on image size and filename
//std::string FileWriter::createPPMFile(std::string filename, const ImageSize& imageSize) {
//    // Change the file extension to .ppm
//    filename.pop_back(); filename.pop_back(); filename.pop_back();
//    filename.append("ppm");
//
//    // Open an output file stream
//    std::ofstream outfile(filename);
//    if (!outfile.is_open()) {
//        throw std::runtime_error("Error: Unable to create PPM file: " + filename);
//    }
//
//    // Write the PPM file header
//    outfile << "P3" << "\n" << imageSize.getWidth() << " " << imageSize.getHeight() << "\n" << "255\n";
//    outfile.close();
//
//    return filename;
//}