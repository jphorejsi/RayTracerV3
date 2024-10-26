#pragma once
#include <fstream>
#include <sstream>
#include "camera.h"
#include "scene.h"
#include "ImageSize.h"


class FileReaderType {
	//readFile helpers
	void processEye(std::istringstream& iss, CameraType& camera);
	void processViewDir(std::istringstream& iss, CameraType& camera);
	void processUpDir(std::istringstream& iss, CameraType& camera);
	void processHfov(std::istringstream& iss, CameraType& camera);
	void processBackgroundColor(std::istringstream& iss, SceneType& scene);
	void processImageSize(std::istringstream& iss, ImageSizeType& imageSize);
	MaterialType* processMaterial(std::istringstream& iss);
	void processSphere(std::istringstream& iss, MaterialType* currentMaterial, TextureType* currentTexture, SceneType& scene);
	void processLight(std::istringstream& iss, SceneType& scene);
	void processAttLight(std::istringstream& iss, SceneType& scene);
	TextureType* processTexture(std::istringstream& iss);
	void processVertex(std::istringstream& iss, std::vector<Vec3*>& vertices);
	void processVertexNormal(std::istringstream& iss, std::vector<Vec3*>& normalVectors);
	void processTextureCoordinate(std::istringstream& iss, std::vector<Vec2*>& textureCoordinates);
	void processFace(const std::string& line, MaterialType* currentMaterial, TextureType* currentTexture, const std::vector<Vec3*>& vertices, const std::vector<Vec3*>& normalVectors, const std::vector<Vec2*>& textureCoordinates, SceneType& scene);

public:
	// Constructor
	FileReaderType() = default;

	// Getters

	// Setters

	// Other methods
	int readFile(const std::string& filename, CameraType& camera, SceneType& scene, ImageSizeType& imageSize);
};


class FileWriterType {
public:
	// Constructor
	FileWriterType() = default;

	// Getters

	// Setters

	// Other methods
	std::string createPPMFile(std::string filename, const ImageSizeType& imageSize);
	//void RenderToPPM(const std::string& filename, const ImageSizeType& imageSize, const ViewFrustrumType& viewFrustrum, const CameraType& camera, const SceneType& scene);
};