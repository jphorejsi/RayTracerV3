#include "shapes.h"
#include <cmath>
#include <stdexcept>

// Determine if ray intersects sphere
bool Sphere::intersects(const Ray& ray, Vec3& intersectionPoint) const {
    Vec3 oc = ray.origin - this->position;
    double a = 1.0; //ray.getDirection().dot(ray.getDirection())
    double b = 2.0f * dot(oc, ray.getDirection());
    //double c = oc.dot(oc) - this->radius * this->radius;
    double c = pow(ray.origin.x - this->position.x, 2) + pow(ray.origin.y - this->position.y, 2) + pow(ray.origin.z - this->position.z, 2) - pow(this->radius, 2);
    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false;

    double t1 = (-b - sqrt(discriminant)) / (2.0f * a);
    double t2 = (-b + sqrt(discriminant)) / (2.0f * a);

    double min = 1e-6;
    double max = FLT_MAX;

    // Check if both roots are invalid
    if ((t1 < min || t1 > max) && (t2 < min || t2 > max)) {
        return false; // No valid intersections
    }

    // Choose the closest valid intersection
    double t = FLT_MAX;
    if (t1 >= min && t1 <= max) {
        t = t1;
    } else if (t2 >= min && t2 <= max) {
        t = t2;
    }

    // Ensure we have a valid t value
    if (t == FLT_MAX) return false;

    // Compute the intersection point
    intersectionPoint = ray.origin + ray.getDirection() * t;
    return true;
}


Vec2 Sphere::getTextureCoordinate(const Vec3& intersectionPoint) const {
    Vec3 localPoint = (intersectionPoint - position).normal();

    // Compute u and v texture coordinates
    double u = 0.5 + (atan2(localPoint.y, localPoint.x) / (2 * 3.1415927));
    double v = 0.5 - (asin(localPoint.z) / 3.1415927);

    return Vec2(u, v);
}

// calculate normal at intersectionPoint
Vec3 Sphere::getNormal(const Vec3& intersectionPoint, Vec2 textureCoordinates) const { //  TODO
    return (intersectionPoint - this->position).normal();
}

// Determine if ray intersects triangle
bool Triangle::intersects(const Ray& ray, Vec3& intersectionPoint) const {
    Vec3 edgeAB = this->vertexB - this->vertexA;
    Vec3 edgeAC = this->vertexC - this->vertexA;
    Vec3 normal = cross(edgeAB, edgeAC).normal();

    double discriminant = dot(normal, ray.getDirection());
    if (fabs(discriminant) < 1e-6f) return false;

    double D = dot(-normal, this->vertexA);
    double ray_t = -(dot(normal, ray.origin) + D) / discriminant;
    if (ray_t < 1e-5f) return false;

    intersectionPoint = ray.origin + ray.getDirection() * ray_t;

    // Barycentric coordinates for the intersection point
    Vec3 v0 = this->vertexB - this->vertexA;
    Vec3 v1 = this->vertexC - this->vertexA;
    Vec3 v2 = intersectionPoint - this->vertexA;

    double dot00 = dot(v0, v0);
    double dot01 = dot(v0, v1);
    double dot02 = dot(v0, v2);
    double dot11 = dot(v1, v1);
    double dot12 = dot(v1, v2);

    double invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    double v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return (u >= 0 && v >= 0 && (u + v) <= 1);
}


Vec2 Triangle::getTextureCoordinate(const Vec3& intersectionPoint) const {
    if (!textureCoordinateA || !textureCoordinateB || !textureCoordinateC) {
        return Vec2(-1, -1);
    }
    Vec3 barycentricCoordinates = getBarycentricCoordinates(intersectionPoint);
    double u = barycentricCoordinates.x * textureCoordinateA->x + barycentricCoordinates.y * textureCoordinateB->x + barycentricCoordinates.z * textureCoordinateC->x;
    double v = barycentricCoordinates.x * textureCoordinateA->y + barycentricCoordinates.y * textureCoordinateB->y + barycentricCoordinates.z * textureCoordinateC->y;
    return Vec2(u, v);
}

// calculate normal at intersectionPoint
Vec3 Triangle::getNormal(const Vec3& intersectionPoint, Vec2 textureCoordinates) const {
    // Step 1: Interpolated normal
    Vec3 interpolatedNormal;
    if (vertexANormal && vertexBNormal && vertexCNormal) {
        Vec3 baryCoords = getBarycentricCoordinates(intersectionPoint);
        interpolatedNormal =
            baryCoords.x * (*vertexANormal) +
            baryCoords.y * (*vertexBNormal) +
            baryCoords.z * (*vertexCNormal);
        interpolatedNormal = interpolatedNormal.normal(); // Normalize the interpolated normal
    }
    else {
        // Fallback: geometric normal
        interpolatedNormal = (cross(vertexB - vertexA, vertexC - vertexA)).normal();
    }

    // Step 2: If no normal map is provided, return the interpolated normal
    if (!this->getMaterial().getNormalMap()) {
        return interpolatedNormal;
    }

    // Step 3: Tangent and bitangent computation using UV coordinates
    if (textureCoordinateA && textureCoordinateB && textureCoordinateC) {
        Vec3 edge1 = vertexB - vertexA;
        Vec3 edge2 = vertexC - vertexA;

        Vec2 deltaUV1 = *textureCoordinateB - *textureCoordinateA;
        Vec2 deltaUV2 = *textureCoordinateC - *textureCoordinateA;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        // Tangent and bitangent calculation
        Vec3 tangent;
        Vec3 bitangent;

        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        tangent = tangent.normal();
        bitangent = bitangent.normal();

        // Step 4: Retrieve tangent-space normal from the normal map
        Vec3 tangentSpaceNormal = this->getMaterial().getNormalMap()->getNormal(textureCoordinates);

        // Negate the y-component of the tangent-space normal
        tangentSpaceNormal.y = -tangentSpaceNormal.y;

        // Step 5: Transform tangent-space normal to world space
        Vec3 worldNormal =
            tangent * tangentSpaceNormal.x +
            bitangent * tangentSpaceNormal.y +
            interpolatedNormal * tangentSpaceNormal.z;

        return worldNormal.normal(); // Return the normalized world-space normal
    }

    // Step 6: If texture coordinates are unavailable, return the interpolated normal
    return interpolatedNormal;
}


Vec3 Triangle::getBarycentricCoordinates(const Vec3& intersectionPoint) const {
    // Compute edges
    Vec3 e1 = vertexB - vertexA; // Edge 1: vertexB - vertexA
    Vec3 e2 = vertexC - vertexA; // Edge 2: vertexC - vertexA
    Vec3 ep = intersectionPoint - vertexA; // Vector from vertexA to intersectionPoint

    // Dot products
    double d11 = dot(e1, e1); // e1 • e1
    double d12 = dot(e1, e2); // e1 • e2
    double d22 = dot(e2, e2); // e2 • e2
    double dp1 = dot(ep, e1); // ep • e1
    double dp2 = dot(ep, e2); // ep • e2

    // Compute denominator
    double denom = d11 * d22 - d12 * d12;
    if (std::fabs(denom) < 1e-6) { // Add epsilon check for precision issues
        throw std::runtime_error("Error: Degenerate triangle with zero area.");
    }

    // Compute barycentric coordinates
    double beta = (d22 * dp1 - d12 * dp2) / denom;  // β = D_beta / denom
    double gamma = (d11 * dp2 - d12 * dp1) / denom; // γ = D_gamma / denom
    double alpha = 1.0 - beta - gamma;             // α = 1 - β - γ

    return Vec3(alpha, beta, gamma); // Return barycentric coordinates
}

// Mesh constructor
Mesh::Mesh(const std::string& filename, const std::shared_ptr<Material>& material) {
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
    }
    file.close();
}

// Return the faces in mesh
const std::vector<Triangle> Mesh::getFaces() const {
    return this->faces;
}

// Read an obj file into a mesh
void Mesh::processFace(std::string& line, const std::shared_ptr<Material>& material) {
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
