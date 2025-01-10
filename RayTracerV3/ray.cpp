#include "ray.h"
#include <stdexcept>

Ray::Ray(Vec3 origin, Vec3 direction) {
	if (direction.length() == 0.0) {
		throw std::invalid_argument("Direction magnitude must be greater than 0.");
	}
	this->direction = direction.normal();
	this->origin = origin;
}

void Ray::setDirection(const Vec3& direction) {
	if (direction.length() == 0.0) {
		throw std::invalid_argument("Direction magnitude must be greater than 0.");
	}
	this->direction = direction.normal();
}