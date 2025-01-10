#pragma once
#include "shapes.h"


class Mesh { // update
private:
    std::vector<Vec3> vertices;
    std::vector<Vec3> vertexNormals;
    std::vector<Vec2> textureCoordinates;
    std::vector<Triangle> faces;

public:
    Mesh(const std::string& filename, const std::shared_ptr<Material>& material) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open OBJ file: " + filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            // Skip empty lines
            if (line.empty()) continue;

            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                // Vertex position
                Vec3 position;
                iss >> position.x >> position.y >> position.z;
                vertices.push_back(position);
            }
            else if (prefix == "vt") {
                // Texture coordinate
                Vec2 texCoord;
                iss >> texCoord.x >> texCoord.y;
                textureCoordinates.push_back(texCoord);
            }
            else if (prefix == "vn") {
                // Vertex normal
                Vec3 normal;
                iss >> normal.x >> normal.y >> normal.z;
                vertexNormals.push_back(normal);
            }
            else if (prefix == "f") {
                // Face
                processFace(line, material);
            }
            // You can handle other prefixes like "g", "o", "s", etc., if needed
        }
        file.close();
    }

    const std::vector<Triangle> getFaces() const {
        return this->faces;
    }

    void processFace(std::string& line, const std::shared_ptr<Material>& material) {
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

        // Parse the input line
        if (sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertexAIndex, &textureCoordinateAIndex, &vertexANormalIndex, &vertexBIndex, &textureCoordinateBIndex, &vertexBNormalIndex, &vertexCIndex, &textureCoordinateCIndex, &vertexCNormalIndex) == 9) {
            // Full data with vertices, texture coordinates, and normals
            textureCoordinateA = &this->textureCoordinates[textureCoordinateAIndex - 1];
            textureCoordinateB = &this->textureCoordinates[textureCoordinateBIndex - 1];
            textureCoordinateC = &this->textureCoordinates[textureCoordinateCIndex - 1];
            vertexANormal = &this->vertexNormals[vertexANormalIndex - 1];
            vertexBNormal = &this->vertexNormals[vertexBNormalIndex - 1];
            vertexCNormal = &this->vertexNormals[vertexCNormalIndex - 1];
        }
        else if (sscanf_s(line.c_str(), "f %d/%d %d/%d %d/%d", &vertexAIndex, &textureCoordinateAIndex, &vertexBIndex, &textureCoordinateBIndex, &vertexCIndex, &textureCoordinateCIndex) == 6) {
            // Vertices with texture coordinates only
            textureCoordinateA = &this->textureCoordinates[textureCoordinateAIndex - 1];
            textureCoordinateB = &this->textureCoordinates[textureCoordinateBIndex - 1];
            textureCoordinateC = &this->textureCoordinates[textureCoordinateCIndex - 1];
        }
        else if (sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", &vertexAIndex, &vertexANormalIndex, &vertexBIndex, &vertexBNormalIndex, &vertexCIndex, &vertexCNormalIndex) == 6) {
            // Vertices with normals only
            vertexANormal = &this->vertexNormals[vertexANormalIndex - 1];
            vertexBNormal = &this->vertexNormals[vertexBNormalIndex - 1];
            vertexCNormal = &this->vertexNormals[vertexCNormalIndex - 1];
        }
        else if (sscanf_s(line.c_str(), "f %d %d %d", &vertexAIndex, &vertexBIndex, &vertexCIndex) == 3) {}
        else { throw std::runtime_error("Error: Triangle parameters are invalid."); }

        Triangle triangle(this->vertices[vertexAIndex - 1], this->vertices[vertexBIndex - 1], this->vertices[vertexCIndex - 1], material);
        triangle.setTextureCoordinateA(textureCoordinateA);
        triangle.setTextureCoordinateB(textureCoordinateB);
        triangle.setTextureCoordinateC(textureCoordinateC);
        triangle.setVertexANormal(vertexANormal);
        triangle.setVertexBNormal(vertexBNormal);
        triangle.setVertexCNormal(vertexCNormal);

        this->faces.push_back(triangle);
    }
};