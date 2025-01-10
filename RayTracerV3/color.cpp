#include "color.h"
#include <stdexcept>

Color::Color(double r, double g, double b) {
    if (r < 0.0 || r > 1.0) {
        throw std::invalid_argument("Red component must be between 0.0 and 1.0.");
    }
    if (g < 0.0 || g > 1.0) {
        throw std::invalid_argument("Green component must be between 0.0 and 1.0.");
    }
    if (b < 0.0 || b > 1.0) {
        throw std::invalid_argument("Blue component must be between 0.0 and 1.0.");
    }
    this->r = r;
    this->g = g;
    this->b = b;
}

void Color::setR(double r) {
    if (r < 0.0 || r > 1.0) {
        throw std::invalid_argument("Red component must be between 0.0 and 1.0.");
    }
    this->r = r;
}

void Color::setG(double g) {
    if (g < 0.0 || g > 1.0) {
        throw std::invalid_argument("Green component must be between 0.0 and 1.0.");
    }
    this->g = g;
}

void Color::setB(double b) {
    if (b < 0.0 || b > 1.0) {
        throw std::invalid_argument("Blue component must be between 0.0 and 1.0.");
    }
    this->b = b;
}
