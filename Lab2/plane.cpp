#include "plane.h"

Plane::Plane(Vec3f &normal, float d_, Material *m) {
    n = normal;
    d = d_;
    Mat = m;
}

bool Plane::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f direction = r.getDirection();
    Vec3f origin = r.getOrigin();
    if (direction.Dot3(n) == 0) return false;
    float t = (d - origin.Dot3(n)) / (direction.Dot3(n));
    if (t > tmin && t < h.getT()) {
        h.set(t, Mat, n, r);
        return true;
    }
    else return false;
}