#include "materials.h"
#include "scene.h"
#include "lights.h"

Color PhongMaterial::calculateColor(const Scene& scene) const {
	Color colorToReturn = this->od * this->ka;
	for (const ILight* light : scene.getLights()) {
		
	}
	return Color(1, 1, 1);
}