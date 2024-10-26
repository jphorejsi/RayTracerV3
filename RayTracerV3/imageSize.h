#pragma once

class ImageSizeType {
protected:
	int width, height;

public:
	// Constructor
	ImageSizeType() = default;
	ImageSizeType(int width, int height) : width(width), height(height) {}

	// Getters
	int getWidth() const { return width; }
	int getHeight() const { return height; }

	// Setters
	void setWidth(int width) { this->width = width; }
	void setHeight(int height) { this->height = height; }
	void setSize(int width, int height) { setWidth(width), setHeight(height); }

	// Other methods
};