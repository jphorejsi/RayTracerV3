#pragma once
class ColorType {
	float r, g, b;

public:
	// Constructor
	ColorType(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b) {}

	// Getters
	float getR() const { return r; }
	float getG() const { return g; }
	float getB() const { return b; }

	// Setters
	void setR(float r) { this->r = r; }
	void setG(float g) { this->g = g; }
	void setB(float b) { this->b = b; }

	// Other methods
};