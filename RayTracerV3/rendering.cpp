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
    Vec3 intersectionPoint;
    return Color(1, 1, 1);
    //const AbstractShape* closestShape = ray.findClosestIntersectedShape(scene, intersectionPoint);

    // If no intersection found, return the background color
    //if (!closestShape) {
    //    return scene.getBackgroundColor();
    //}


}

