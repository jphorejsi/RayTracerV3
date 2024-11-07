#include "rendering.h"

#include "color.h"
#include "scene.h"
#include "shapes.h"
#include "kdtree.h"
#include <vector>
#include <limits>
#include <algorithm>

// Ray tracing function that traverses the KDTreeNode
Color Rendering::traceRay(const Ray& ray, const Scene& scene, int depth) {
    // Step 1: Find all intersected leaf nodes in the KD tree using the root node's getter
    const KDTreeNode* kdTreeRoot = scene.getKDRoot();
    std::vector<KDTreeNode*> intersectedNodes = kdTreeRoot->findAllIntersectedLeafNodes(ray);
    
    // If no nodes are intersected, return the background color
    if (intersectedNodes.empty()) {
        return scene.getBackgroundColor();
    }

    Vec3 intersectionPoint, closestIntersectionPoint, normal;
    const IShape* closestShape = nullptr;
    float closestDistance = std::numeric_limits<float>::max();
    
    // Step 2: Iterate through all intersected nodes and check for ray-shape intersections using the getter method
    for (const KDTreeNode* node : intersectedNodes) {
        for (const IShape* shape : node->getShapes()) {
            Vec3 tempIntersectionPoint;
            if (shape->intersects(ray, tempIntersectionPoint)) {
                float distance = (tempIntersectionPoint - ray.getOrigin()).length();
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestIntersectionPoint = tempIntersectionPoint;
                    closestShape = shape;
                }
            }
        }
    }

    // If no intersection found with any shape, return the background color
    if (!closestShape) {
        return scene.getBackgroundColor();
    }
    return Color(1, 1, 1);
    //return closestShape->getMaterial()->shadeRay();
}

Color Rendering::shadeRay(const Ray& ray, const Scene& scene, const IShape* shape, const Vec3& intersectionPoint) {
    IMaterial* material = shape->getMaterial();
    if (!material) {
        return scene.getBackgroundColor(); // Return background color if no material is set
    }
    if (shape->getTexture()) {
        Vec2 uv = shape->calculateTextureCoordinate(intersectionPoint);
        int u = uv.getX() * (shape->getTexture()->getImageSize().getWidth() - 1);
        int v = uv.getY() * (shape->getTexture()->getImageSize().getHeight() - 1);
        material->setColor(shape->getTexture()->getPixel(u, v));
    }

    // Call the material's shade method, which performs the correct shading based on its type
    //Vec3 normal = shape->getNormal(intersectionPoint); // Assuming `getNormal` gives surface normal
    return material->calculateColor(scene);
}
