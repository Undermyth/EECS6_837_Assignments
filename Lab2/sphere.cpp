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
    /* t2 be the intersect */
    if (t2 > tmin && t2 < h.getT()) {
        Vec3f normal = relative_origin + t2 * r.getDirection();
        normal.Normalize();
        h.set(t2, Mat, normal, r);
        return true;
    }
    /* t1 be the intersect */
    else if (t1 > tmin && t1 < h.getT()) {
        Vec3f normal = relative_origin + t1 * r.getDirection();
        normal.Normalize();
        h.set(t1, Mat, normal, r);
        return true;
    }
    else return false;
}