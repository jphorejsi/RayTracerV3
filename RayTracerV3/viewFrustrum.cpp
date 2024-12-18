#include "viewFrustrum.h"

ViewFrustrum::ViewFrustrum(const Camera& camera, const ImageSize& imageSize) {
    // get the aspect ratio (width/height) to ensure the window matches the image's proportions.
    double aspectRatio = (double)imageSize.getWidth() / (double)imageSize.getHeight();

    // Compute the window width based on the camera's horizontal field of view (FOV).
    // - Convert FOV from degrees to radians: (FOV * pi / 180)
    // - get the tangent of half the FOV to determine the size of the view frustum at a depth of 1 unit.
    // - Multiply by 2 to get the full width of the viewing window.
    this->width = 2 * 5 * tan(0.5 * (camera.getHorizontalFOV() * 3.14159265358979323846 / 180.0));

    // get the height of the viewing window using the aspect ratio.
    this->height = this->width / aspectRatio;

    // get the camera's local x-axis (u) by taking the cross product of the view direction and the up direction.
    // - Normalize the result to get a unit vector.
    // - Scale by half the window width to get the u-axis extent.
    Vec3 uNormal = camera.getViewDirection().cross(camera.getUpDirection()).normal();
    Vec3 u = uNormal * (this->width / 2.0f);

    // get the camera's local y-axis (v) by taking the cross product of u and the view direction.
    // - Normalize the result to get a unit vector.
    // - Scale by half the window height to get the v-axis extent.
    Vec3 v = (uNormal.cross(camera.getViewDirection()).normal()) * (this->height / 2.0f);


    // get the center point of the viewing plane, which is 1 unit in front of the camera along the view direction.
    Vec3 viewPlaneCenter = camera.getEyePosition() + camera.getViewDirection() * 5.0f;

    // Compute the four corners of the viewing window.
    this->upperLeft = viewPlaneCenter - u + v;
    this->upperRight = viewPlaneCenter + u + v;
    this->lowerLeft = viewPlaneCenter - u - v;
    this->lowerRight = viewPlaneCenter + u - v;
}