#pragma once

// Color class to represent RGB color values
class Color {
private:
    double r, g, b;  // RGB components

public:
    // Constructor
    Color(double r = 0.0f, double g = 0.0f, double b = 0.0f) : r(r), g(g), b(b) {}

    // Getters
    double getR() const { return this->r; }
    double getG() const { return this->g; }
    double getB() const { return this->b; }

    // Setters
    void setR(double red) { this->r = red; }
    void setG(double green) { this->g = green; }
    void setB(double blue) { this->b = blue; }

    // Other methods
    void normalize() {
        this->r = r / 255.0;
        this->g = g / 255.0;
        this->b = b / 255.0;
    }

    // Operator overloads
    Color operator+(const Color& color) const { return Color(this->r + color.r, this->g + color.g, this->b + color.b); }
    Color operator*(double scalar) const { return Color(this->r * scalar, this->g * scalar, this->b * scalar); }
    Color operator*(const Color& color) const { return Color(this->r * color.r, this->g * color.g, this->b * color.b); }

    Color& operator+=(const Color& color) {
        this->r += color.r;
        this->g += color.g;
        this->b += color.b;
        return *this;
    }
};

// Overload for scalar multiplication from the left
inline Color operator*(double scalar, const Color& color) {
    return Color(color.getR() * scalar, color.getG() * scalar, color.getB() * scalar);
}
