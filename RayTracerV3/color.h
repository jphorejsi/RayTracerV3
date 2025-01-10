#pragma once

#include <stdexcept>

// Color class to represent RGB color values
class Color {
private:
    double r, g, b;  // RGB components restricted to 0 1

public:
    // Constructors
    Color() : r(0.0), g(0.0), b(0.0) {}
    Color(double r, double g, double b);

    // Getters
    double getR() const { return this->r; }
    double getG() const { return this->g; }
    double getB() const { return this->b; }

    // Setters
    void setR(double red);
    void setG(double green);
    void setB(double blue);

    // Operator overloads as member functions
    Color& operator+=(const Color& color){
        this->r += color.r;
        this->g += color.g;
        this->b += color.b;

        // Clamp the values between 0.0 and 1.0
        if (this->r > 1.0) this->r = 1.0;
        if (this->r < 0.0) this->r = 0.0;

        if (this->g > 1.0) this->g = 1.0;
        if (this->g < 0.0) this->g = 0.0;

        if (this->b > 1.0) this->b = 1.0;
        if (this->b < 0.0) this->b = 0.0;

        return *this;
    }
};

// Free function operator overloads

// Operator+ : Adds two Colors and returns a new Color with clamped values
inline Color operator+(const Color& a, const Color& b) {
    double newR = a.getR() + b.getR();
    double newG = a.getG() + b.getG();
    double newB = a.getB() + b.getB();

    // Clamp the values between 0.0 and 1.0
    if (newR > 1.0) newR = 1.0;
    if (newR < 0.0) newR = 0.0;

    if (newG > 1.0) newG = 1.0;
    if (newG < 0.0) newG = 0.0;

    if (newB > 1.0) newB = 1.0;
    if (newB < 0.0) newB = 0.0;

    return Color(newR, newG, newB);
}

// Operator* : Multiplies Color by a scalar and returns a new Color with clamped values
inline Color operator*(const Color& color, double scalar) {
    double newR = color.getR() * scalar;
    double newG = color.getG() * scalar;
    double newB = color.getB() * scalar;

    // Clamp the values between 0.0 and 1.0
    if (newR > 1.0) newR = 1.0;
    if (newR < 0.0) newR = 0.0;

    if (newG > 1.0) newG = 1.0;
    if (newG < 0.0) newG = 0.0;

    if (newB > 1.0) newB = 1.0;
    if (newB < 0.0) newB = 0.0;

    return Color(newR, newG, newB);
}

// Operator* : Multiplies two Colors component-wise and returns a new Color with clamped values
inline Color operator*(const Color& a, const Color& b) {
    double newR = a.getR() * b.getR();
    double newG = a.getG() * b.getG();
    double newB = a.getB() * b.getB();

    // Clamp the values between 0.0 and 1.0
    if (newR > 1.0) newR = 1.0;
    if (newR < 0.0) newR = 0.0;

    if (newG > 1.0) newG = 1.0;
    if (newG < 0.0) newG = 0.0;

    if (newB > 1.0) newB = 1.0;
    if (newB < 0.0) newB = 0.0;

    return Color(newR, newG, newB);
}

// Operator* : Enables scalar multiplication with scalar on the left
inline Color operator*(double scalar, const Color& color) {
    return color * scalar; // Reuse the existing operator*
}