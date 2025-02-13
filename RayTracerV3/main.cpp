#include "scene.h"
#include "camera.h"
#include "renderer.h"
#include "fileManager.h"
#include <crtdbg.h>

void static basicLighting() {
	Scene scene(Color(0.1, 0.1, 0.1));
	Camera cam(Vec3(0, 0, 0), Vec3(0, 1, 0), Vec3(0, 0.1, -1), 90);
	Renderer system(512, 512);

	AbstractLight* light1 = new DirectionalLight(Vec3(-1, -1, -1), Color(0.9, 0.5, 0.05));
	scene.addLight(light1);

	std::shared_ptr<Material> material1 = std::make_shared<BlinnPhong>(Color(1, 0, 0), Color(1, 1, 1), 0.6, 0.2, 0.2, 10);
	Shape* sphere1 = new Sphere(Vec3(0, 1.5, -4), 1, material1);
	scene.addShape(sphere1);

	std::shared_ptr<Material> material2 = std::make_shared<BlinnPhong>(Color(0, 1, 0), Color(1, 1, 1), 0.1, 0.8, 0.2, 10);
	Shape* sphere2 = new Sphere(Vec3(-1.275, -0.75, -4), 1, material2);
	scene.addShape(sphere2);

	std::shared_ptr<Material> material3 = std::make_shared<BlinnPhong>(Color(0, 0, 1), Color(1, 1, 1), 0.1, 0.2, 0.8, 10);
	Shape* sphere3 = new Sphere(Vec3(1.275, -0.75, -4), 1, material3);
	scene.addShape(sphere3);

	system.render(scene, cam);
}

void static attenuation() {
	Scene scene(Color(0, 0, 0));
	Camera cam(Vec3(0, 2, 5), Vec3(0, 1, 0), Vec3(0, -0.25, -1), 45);
	Renderer system(512, 512);

	AbstractLight* light1 = new AttributePointLight(Vec3(8, 8, 10), Color(1, 0, 0), 0.0025, 0.0025, 0.005);
	scene.addLight(light1);

	std::shared_ptr<Material> material1 = std::make_shared<BlinnPhong>(Color(1, 0, 0), Color(1, 1, 1), 0.1, 0.5, 0.3, 20);

	Shape* sphere1 = new Sphere(Vec3(0, -0.2, 0), 1, material1);
	scene.addShape(sphere1);

	Shape* sphere2 = new Sphere(Vec3(0, 0, -10), 1, material1);
	scene.addShape(sphere2);

	Shape* sphere3 = new Sphere(Vec3(0, 1.5, -20), 1, material1);
	scene.addShape(sphere3);

	Sphere* sphere4 = new Sphere(Vec3(0, 4, -30), 1, material1);
	scene.addShape(sphere4);

	system.render(scene, cam);
}

void static earth() {
	Scene scene(Color(0.5, 0.7, 0.9));
	Camera cam(Vec3(2, -6, 1), Vec3(0, 0, 1), Vec3(-1, 3, -0.5), 50);
	Renderer system(512, 512);

	AbstractLight* light1 = new DirectionalLight(Vec3(0, 1, -1), Color(1, 1, 1));
	scene.addLight(light1);

	std::shared_ptr<Material> material1 = std::make_shared<BlinnPhong>(Color(0, 1, 0), Color(1, 1, 1), 0.2, 0.8, 0.1, 20);
	std::shared_ptr<Texture> texture1 = std::make_shared<Texture>("earthtexture.ppm");
	material1->setTexture(texture1);

	Shape* sphere1 = new Sphere(Vec3(0, 0, 0), 2, material1);
	scene.addShape(sphere1);

	system.render(scene, cam);
}

void static earthAA() {
	Scene scene(Color(0.5, 0.7, 0.9));
	Camera cam(Vec3(2, -6, 1), Vec3(0, 0, 1), Vec3(-1, 3, -0.5), 50);
	Renderer system(512, 512);

	system.setSamples(10);

	AbstractLight* light1 = new DirectionalLight(Vec3(0, 1, -1), Color(1, 1, 1));
	scene.addLight(light1);

	std::shared_ptr<Material> material1 = std::make_shared<BlinnPhong>(Color(0, 1, 0), Color(1, 1, 1), 0.2, 0.8, 0.1, 20);
	std::shared_ptr<Texture> texture1 = std::make_shared<Texture>("earthtexture.ppm");
	material1->setTexture(texture1);

	Shape* sphere1 = new Sphere(Vec3(0, 0, 0), 2, material1);
	scene.addShape(sphere1);

	system.render(scene, cam);
}

void static normalMap() {
	Scene scene(Color(0.1, 0.1, 0.1));
	Camera cam(Vec3(0, 0, 2), Vec3(0, 1, 0), Vec3(0, 0, -1), 60);
	Renderer system(1000, 1000);

	AbstractLight* light1 = new PointLight(Vec3(1, 1, 1), Color(1, 0, 0));
	scene.addLight(light1);

	AbstractLight* light2 = new PointLight(Vec3(-1, -1, 1), Color(0, 0, 1));
	scene.addLight(light2);

	std::shared_ptr<Material> material1 = std::make_shared<BlinnPhong>(Color(0.1, 0.1, 0.1), Color(1, 1, 1), 1, 1, 1, 20);
	std::shared_ptr<Texture> texture1 = std::make_shared<Texture>("brickwall.ppm");
	std::shared_ptr<NormalMap> normalMap1 = std::make_shared<NormalMap>("brickwall_normal.ppm");
	material1->setTexture(texture1);
	material1->setNormalMap(normalMap1);

	Mesh mesh1("wall.txt", material1);
	scene.addMesh(mesh1);

	system.render(scene, cam);
}

void static teapot() {
	Scene scene(Color(0, 0, 0));
	Camera cam(Vec3(6, 6, 6), Vec3(-1, 2, -1), Vec3(-1, -1, -1), 45);
	Renderer system(512, 512);

	AbstractLight* light1 = new DirectionalLight(Vec3(0, -0.75, 1), Color(1, 1, 1));
	scene.addLight(light1);

	std::shared_ptr<Material> material1 = std::make_shared<BlinnPhong>(Color(1, 0, 0), Color(1, 1, 1), 0.1, 0.5, 0.3, 20);

	Mesh mesh1("teapot.txt", material1);
	scene.addMesh(mesh1);

	system.render(scene, cam);
}

void static reflective() {
	Scene scene(Color(0.5, 0.7, 0.9)); // another value of 1 for both reflection files
	Camera cam(Vec3(0, 5, 0), Vec3(0, 1, 0), Vec3(0, 0, 1), 45);
	Renderer system(1080, 1080);

	AbstractLight* light1 = new DirectionalLight(Vec3(0, -1, 0), Color(1, 1, 1));
	scene.addLight(light1);

	std::shared_ptr<Material> material1 = std::make_shared<ReflectiveBlinnPhong>(Color(1, 1, 1), Color(1, 1, 1), 0.2, 0.4, 0.6, 60, 0.2, 1.5);

	Shape* sphere1 = new Sphere(Vec3(1.25, 8, 15), 1, material1);
	scene.addShape(sphere1);

	Shape* sphere2 = new Sphere(Vec3(0, 6, 15), 1, material1);
	scene.addShape(sphere2);

	Shape* sphere3 = new Sphere(Vec3(1.5, 4, 15), 1, material1);
	scene.addShape(sphere3);

	std::shared_ptr<Material> material2 = std::make_shared<ReflectiveBlinnPhong>(Color(1, 1, 1), Color(1, 1, 1), 0.2, 0.4, 0.6, 60, 1, 0);

	Shape* sphere4 = new Sphere(Vec3(-1.5, 4, 15), 1, material2);
	scene.addShape(sphere4);

	std::shared_ptr<Material> material3 = std::make_shared<ReflectiveBlinnPhong>(Color(1, 1, 1), Color(1, 1, 1), 0.2, 0.8, 0, 20, 1, 0);

	Mesh mesh1("floor.txt", material3);
	scene.addMesh(mesh1);

	system.render(scene, cam);
}

void static reflective2() {
	Scene scene(Color(0.5, 0.7, 0.9));
	Camera cam(Vec3(0, 0, 0), Vec3(0, 0, 1), Vec3(1, 0, 0), 60);
	Renderer system(1080, 1080);

	AbstractLight* light1 = new PointLight(Vec3(0, 94820000, -28450000), Color(0, 0, 0));
	scene.addLight(light1);

	std::shared_ptr<Material> material1 = std::make_shared<ReflectiveBlinnPhong>(Color(1, 1, 1), Color(1, 1, 1), 0, 0.05, 0.1, 80, 1, 30);

	Shape* sphere1 = new Sphere(Vec3(3, 0, 0), 1, material1);
	scene.addShape(sphere1);

	std::shared_ptr<Material> material2 = std::make_shared<ReflectiveBlinnPhong>(Color(0, 1, 0), Color(1, 1, 1), 1, 0, 0, 1, 1, 0);
	std::shared_ptr<Texture> texture1 = std::make_shared<Texture>("harbor.ppm");
	material2->setTexture(texture1);

	Shape* sphere2 = new Sphere(Vec3(0, 0, 0), 100000000, material2);
	scene.addShape(sphere2);

	system.render(scene, cam);
}

void static depthcue() {
	Scene scene(Color(1, 1, 1));
	Camera cam(Vec3(0, 2, 5), Vec3(0, 1, 0), Vec3(0, -0.25, -1), 45);
	Renderer system(512, 512);

	DepthCue* depthcue = new DepthCue(Color(1, 1, 1), 1.0, 0.2, 30, 5);
	scene.setDepthCue(depthcue);

	AbstractLight* light1 = new DirectionalLight(Vec3(-1, -1, -1), Color(0.9, 0.5, 0.05));
	scene.addLight(light1);

	std::shared_ptr<Material> material1 = std::make_shared<BlinnPhong>(Color(1, 0, 0), Color(1, 1, 1), 0.1, 0.5, 0.3, 20);

	Shape* sphere1 = new Sphere(Vec3(0, -0.2, 0), 1, material1);
	scene.addShape(sphere1);

	Shape* sphere2 = new Sphere(Vec3(0, 0, -10), 1, material1);
	scene.addShape(sphere2);

	Shape* sphere3 = new Sphere(Vec3(0, 1.5, -20), 1, material1);
	scene.addShape(sphere3);

	Sphere* sphere4 = new Sphere(Vec3(0, 4, -30), 1, material1);
	scene.addShape(sphere4);

	system.render(scene, cam);
}

void static fileRead(std::string filename) {
	Scene scene;
	Camera cam;
	Renderer system;

	FileReader fr;
	fr.readFile(filename, cam, scene, system);
	
	system.render(scene, cam);
}

int main() {
	fileRead("input.txt");
}

