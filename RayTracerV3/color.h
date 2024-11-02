#pragma once

// Color class to represent RGB color values
class Color {
private:
    float r, g, b;  // RGB components

public:
    // Constructor
    Color(float r = 0.0f, float g = 0.0f, float b = 0.0f) : r(r), g(g), b(b) {}

    // Getters
    float getR() const { return this->r; }
    float getG() const { return this->g; }
    float getB() const { return this->b; }

    // Setters
    void setR(float red) { this->r = red; }
    void setG(float green) { this->g = green; }
    void setB(float blue) { this->b = blue; }

    // Operator overloads
    Color operator+(const Color& color) const { return Color(this->r + color.r, this->g + color.g, this->b + color.b); }
    Color operator*(float scalar) const { return Color(this->r * scalar, this->g * scalar, this->b * scalar); }
    Color operator*(const Color& color) const { return Color(this->r * color.r, this->g * color.g, this->b * color.b); }
};

