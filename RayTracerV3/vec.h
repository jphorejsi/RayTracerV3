#pragma once
#include <cmath>
#include <stdexcept>

class Vec2 {
private:
	double x, y;

public:
	//constructor
	Vec2(double x = 0.0, double y = 0.0) : x(x), y(y) {}

	// Getters
	double getX() const { return x; }
	double getY() const { return y; }

    // Setters
    void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }

    // Other methods

    // operator overloads
    double& operator[](int index) {
        if (index == 0) return x;
        else if (index == 1) return y;
        else throw std::out_of_range("Index out of range for Vec2");
    }
};

class Vec3 {
private:
	double x, y, z;

public:
	//constructor
	Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

	// Getters
	double getX() const { return x; }
	double getY() const { return y; }
    double getZ() const { return z; }

    // Setters
    void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setZ(double z) { this->z = z; }

    // Other member functions
    double length() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3 cross(const Vec3& v) const { return Vec3(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x); }
    double dot(const Vec3& v) const { return (this->x * v.x + this->y * v.y + this->z * v.z); }

    Vec3 normal() const {
        double norm = std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
        if (norm == 0) {
            return Vec3(0, 0, 0);
        }
        return Vec3(this->x / norm, this->y / norm, this->z / norm);
    }

    static Vec3 min(const Vec3& a, const Vec3& b) { return Vec3(std::min(a.getX(), b.getX()), std::min(a.getY(), b.getY()), std::min(a.getZ(), b.getZ())); }
    static Vec3 max(const Vec3& a, const Vec3& b) { return Vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)); }

    // Operator overloads
    Vec3 operator+(const Vec3& v) const { return Vec3(this->x + v.x, this->y + v.y, this->z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(this->x - v.x, this->y - v.y, this->z - v.z); }
    Vec3 operator*(double c) const { return Vec3(this->x * c, this->y * c, this->z * c); }
    Vec3 operator/(double c) const { return Vec3(this->x / c, this->y / c, this->z / c); }
    bool operator==(const Vec3& v) const { return this->x == v.x && this->y == v.y && this->z == v.z; }

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

    Vec3 transform(const Vec3& baseNormal) const {
        // Normalize the base normal
        Vec3 normalizedBaseNormal = baseNormal.normal();

        // Check for degenerate baseNormal
        if (normalizedBaseNormal.length() == 0.0) {
            throw std::runtime_error("Error: Base normal is zero-length.");
        }

        // Calculate the tangent vector
        Vec3 tangent;
        if (std::abs(normalizedBaseNormal.getY()) < 1e-6) {
            tangent = Vec3(0, -normalizedBaseNormal.getZ(), normalizedBaseNormal.getY());
        }
        else {
            tangent = Vec3(normalizedBaseNormal.getZ(), 0, -normalizedBaseNormal.getX());
        }

        if (tangent.length() == 0.0) {
            throw std::runtime_error("Error: Degenerate tangent vector.");
        }
        tangent = tangent.normal();

        // Calculate the bitangent vector
        Vec3 bitangent = normalizedBaseNormal.cross(tangent);
        if (bitangent.length() == 0.0) {
            throw std::runtime_error("Error: Degenerate bitangent vector.");
        }
        bitangent = bitangent.normal();

        // Transform the vector from tangent space to world space
        Vec3 transformed(
            this->getX() * tangent.getX() + this->getY() * bitangent.getX() + this->getZ() * normalizedBaseNormal.getX(),
            this->getX() * tangent.getY() + this->getY() * bitangent.getY() + this->getZ() * normalizedBaseNormal.getY(),
            this->getX() * tangent.getZ() + this->getY() * bitangent.getZ() + this->getZ() * normalizedBaseNormal.getZ()
        );

        return transformed.normal(); // Normalize the output for safety
    }

};

inline Vec3 operator*(double a, const Vec3& b) { return Vec3(b.getX() * a, b.getY() * a, b.getZ() * a); }