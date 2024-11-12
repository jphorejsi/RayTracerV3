#pragma once
#include <cmath>

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

    // Operator overloads
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
};

Vec3 operator*(float c, const Vec3& v) {
    return Vec3(v.getX() * c, v.getY() * c, v.getZ() * c);
}