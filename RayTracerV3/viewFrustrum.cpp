#include "viewFrustrum.h"

ViewFrustrum::ViewFrustrum(const Camera& camera, const ImageSize& imageSize) {
    // Calculate the aspect ratio (width/height) to ensure the window matches the image's proportions.
    float aspectRatio = (float)imageSize.getWidth() / (float)imageSize.getHeight();

    // Compute the window width based on the camera's horizontal field of view (FOV).
    // - Convert FOV from degrees to radians: (FOV * pi / 180)
    // - Calculate the tangent of half the FOV to determine the size of the view frustum at a depth of 1 unit.
    // - Multiply by 2 to get the full width of the viewing window.
    this->width = 2 * tan(0.5 * (camera.getHorizontalFOV() * 3.14159265358979323846 / 180.0));

    // Calculate the height of the viewing window using the aspect ratio.
    this->height = this->width / aspectRatio;

    // Calculate the camera's local x-axis (u) by taking the cross product of the view direction and the up direction.
    // - Normalize the result to get a unit vector.
    // - Scale by half the window width to get the u-axis extent.
    Vec3 u = (camera.getViewDirection().cross(camera.getUpDirection())).normal() * (this->width / 2.0f);

    // Calculate the camera's local y-axis (v) by taking the cross product of u and the view direction.
    // - Normalize the result to get a unit vector.
    // - Scale by half the window height to get the v-axis extent.
    Vec3 v = (u.cross(camera.getViewDirection()).normal()) * (this->height / 2.0f);

    // Calculate the center point of the viewing plane, which is 1 unit in front of the camera along the view direction.
    Vec3 viewPlaneCenter = camera.getEyePosition() + camera.getViewDirection();

    // Compute the four corners of the viewing window.
    this->upperLeft = viewPlaneCenter - u + v;
    this->upperRight = viewPlaneCenter + u + v;
    this->lowerLeft = viewPlaneCenter - u - v;
    this->lowerRight = viewPlaneCenter + u - v;
}
