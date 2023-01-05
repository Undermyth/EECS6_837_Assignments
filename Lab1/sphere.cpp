#include "sphere.h"

Sphere::Sphere(Vec3f& c, float r, Material *M) {
    center = c;
    radius = r;
    Mat = M;
}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin) {
    Vec3f relative_origin = r.getOrigin() - center;
    float a = r.getDirection().Length() * r.getDirection().Length();
    float b = 2 * relative_origin.Dot3(r.getDirection());
    float c = relative_origin.Length() * relative_origin.Length() - radius * radius;
    float delta = b * b - 4 * a * c;
    if (delta < 0) return false;
    delta = sqrt(delta);
    float t1 = (-b + delta) / (2 * a), t2 = (-b - delta) / (2 * a);
    if (t2 > tmin && t2 < h.getT()) {
        h.set(t2, Mat, r);
    }
    else if (t1 > tmin && t1 < h.getT()) {
        h.set(t1, Mat, r);
    }
    return true;
}