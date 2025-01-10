#pragma once
#include <cmath>
#include <stdexcept>

class Vec2 {
public:
    double x, y; // can be any value

	//constructor
	Vec2(double x = 0.0, double y = 0.0) : x(x), y(y) {}
};

class Vec3 {
public:
    double x, y, z;

	//constructor
	Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    // Other member functions
    double length() const { return std::sqrt(x * x + y * y + z * z); }

    Vec3 normal() const {
        double norm = std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
        if (norm == 0) {
            return Vec3(0, 0, 0);
        }
        return Vec3(this->x / norm, this->y / norm, this->z / norm);
    }

    static Vec3 min(const Vec3& a, const Vec3& b) { return Vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)); }
    static Vec3 max(const Vec3& a, const Vec3& b) { return Vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)); }

    // Operator overloads
    Vec3& operator+=(const Vec3& v) {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        return *this; // Return the current object by reference
    }

    Vec3& operator-=(const Vec3& v) {
        this->x -= v.x;
        this->y -= v.y;
        this->z -= v.z;
        return *this; // Return the current object by reference
    }

    double& operator[](int index) {
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        throw std::out_of_range("Index out of range for Vec3");
    }

    const double& operator[](int index) const { // Const version for read-only access
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        throw std::out_of_range("Index out of range for Vec3");
    }

    Vec3 operator-() const {
        return Vec3(-x, -y, -z);
    }

};

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vec3 operator*(double t, const Vec3& v) {
    return Vec3(t * v.x, t * v.y, t * v.z);
}

inline Vec3 operator*(const Vec3& v, double t) {
    return t * v;
}

inline Vec3 operator/(const Vec3& v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vec3& u, const Vec3& v) {
    return u.x * v.x
        + u.y * v.y
        + u.z * v.z;
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * dot(v, n) * n;
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}




// Other methods
//Vec3 transform(const Vec3& baseNormal) const {
//    // Normalize the base normal
//    Vec3 normalizedBaseNormal = baseNormal.normal();

//    // Check for degenerate baseNormal
//    if (normalizedBaseNormal.length() == 0.0) {
//        throw std::runtime_error("Error: Base normal is zero-length.");
//    }

//    // Calculate the tangent vector
//    Vec3 tangent;
//    if (std::abs(normalizedBaseNormal.y) < 1e-6) {
//        tangent = Vec3(0, -normalizedBaseNormal.z, normalizedBaseNormal.y);
//    }
//    else {
//        tangent = Vec3(normalizedBaseNormal.z, 0, -normalizedBaseNormal.x);
//    }

//    if (tangent.length() == 0.0) {
//        throw std::runtime_error("Error: Degenerate tangent vector.");
//    }
//    tangent = tangent.normal();

//    // Calculate the bitangent vector
//    Vec3 bitangent = cross(normalizedBaseNormal, tangent);
//    if (bitangent.length() == 0.0) {
//        throw std::runtime_error("Error: Degenerate bitangent vector.");
//    }
//    bitangent = bitangent.normal();

//    // Transform the vector from tangent space to world space
//    Vec3 transformed(
//        this->x * tangent.x + this->y * bitangent.x + this->z * normalizedBaseNormal.x,
//        this->x * tangent.y + this->y * bitangent.y + this->z * normalizedBaseNormal.y,
//        this->x * tangent.z + this->y * bitangent.z + this->z * normalizedBaseNormal.z
//    );

//    return transformed.normal(); // Normalize the output for safety
//}