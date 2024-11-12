#include "materials.h"

#include "scene.h"
#include "shapes.h"

Color PhongMaterial::shade(const Ray& ray, const Vec3& intersectionPoint, const Scene& scene, const AbstractShape* shape) const {
	Color ambient = this->od * this->ka;
	Color diffuse(0, 0, 0);
	Color specular(0, 0, 0);
	Vec3 N = shape->getNormal(intersectionPoint);

	for (AbstractLight* light : scene.getLights()) {
		if (light->illuminates(intersectionPoint)) {
			diffuse += light->calculateDiffuse(this, intersectionPoint, N);
			specular += light->calculateSpecular(this, intersectionPoint, N, ray.getDirection());
		}
	}

	return ambient + diffuse + specular;
}