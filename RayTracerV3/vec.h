#pragma once
#include <cmath>
#include <stdexcept>

class Vec2 {
private:
	float x, y;

public:
	//constructor
	Vec2(float x = 0.0, float y = 0.0) : x(x), y(y) {}

	// Getters
	float getX() const { return x; }
	float getY() const { return y; }

    // Setters
    void setX(float x) { this->x = x; }
	void setY(float y) { this->y = y; }

    // Other methods

    // operator overloads
    float& operator[](int index) {
        if (index == 0) return x;
        else if (index == 1) return y;
        else throw std::out_of_range("Index out of range for Vec2");
    }
};

class Vec3 {
private:
	float x, y, z;

public:
	//constructor
	Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

	// Getters
	float getX() const { return x; }
	float getY() const { return y; }
    float getZ() const { return z; }

    // Setters
    void setX(float x) { this->x = x; }
	void setY(float y) { this->y = y; }
	void setZ(float z) { this->z = z; }

    // Other member functions
    float length() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3 cross(const Vec3& v) const { return Vec3(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x); }
    float dot(const Vec3& v) const { return (this->x * v.x + this->y * v.y + this->z * v.z); }

    Vec3 normal() const {
        float norm = std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
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
    Vec3 operator*(float c) const { return Vec3(this->x * c, this->y * c, this->z * c); }
    Vec3 operator/(float c) const { return Vec3(this->x / c, this->y / c, this->z / c); }
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

    float& operator[](int index) {
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        throw std::out_of_range("Index out of range for Vec3");
    }

    const float& operator[](int index) const { // Const version for read-only access
        if (index == 0) return x;
        if (index == 1) return y;
        if (index == 2) return z;
        throw std::out_of_range("Index out of range for Vec3");
    }
};

inline Vec3 operator*(float a, const Vec3& b) { return Vec3(b.getX() * a, b.getY() * a, b.getZ() * a); }