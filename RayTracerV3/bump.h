#pragma once
#include "vec.h"

class BumpType {
	int width, height, maxValue;
	Vec3** bumpArray;

public:
	// Constructor
	BumpType(int width = 0, int height = 0, int maxValue = 255) : width(width), height(height), maxValue(maxValue) {
		this->bumpArray = new Vec3 * [width];
		for (int i = 0; i < width; i++) {
			this->bumpArray[i] = new Vec3[height];
		}
	}

	// Getters

	// Setters

	// Other methods
};
