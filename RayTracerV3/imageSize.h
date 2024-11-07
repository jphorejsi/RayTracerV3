#pragma once

#include <stdexcept>

class ImageSize {
private:
	int width, height;

public:
	// Constructor
	ImageSize() = default;
	ImageSize(int width, int height) : width(width), height(height) {}

	// Getters
	int getWidth() const { return width; }
	int getHeight() const { return height; }

	// Setters
	void setWidth(int width) {
		if (width <= 0) {
			throw std::invalid_argument("Width must be greater than 0.");
		}
		this->width = width;
	}

	void setHeight(int height) {
		if (height <= 0) {
			throw std::invalid_argument("Height must be greater than 0.");
		}
		this->height = height;
	}

	void setSize(int width, int height) { setWidth(width), setHeight(height); }
};