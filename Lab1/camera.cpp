#include "camera.h"

OrthographicCamera::OrthographicCamera(Vec3f& c, Vec3f& d, Vec3f& u, int s) {
    center = c;
    direction = d;
    up = u;
    size = s;
}   

Ray OrthographicCamera::generateRay(Vec2f point) {
    float x = point.x();
    float y = point.y();    // normalized coordinate

    direction.Normalize();
    up -= direction * up.Dot3(direction);
    up.Normalize();
    Vec3f horizontal;
    horizontal.Cross3(horizontal, direction, up);   // ?

    Vec3f origin = center + (x - 0.5) * size * horizontal + (y - 0.5) * size * up;
    Ray ray = Ray(origin, direction);
    return ray;
}

float OrthographicCamera::getTMin() const{
    return (float)(-3e7);
}
