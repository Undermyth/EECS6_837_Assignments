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

void Plane::paint(void) {
    Mat -> glSetMaterial();
    /* generate two basis on the plane */
    Vec3f v;
    if (n.y() == 0 && n.z() == 0) {
        v = Vec3f(0, 1, 0);
    }
    else {
        v = Vec3f(1, 0, 0);
    }
    Vec3f b1, b2;
    Vec3f::Cross3(b1, v, n);
    Vec3f::Cross3(b2, n, b1);
    b1.Normalize();
    b2.Normalize();
    /* project world origin onto the plane */
    Vec3f projected_origin = d / (n.Length() * n.Length()) * n;
    Vec3f P1 = projected_origin - BIG * b1 - BIG * b2;
    Vec3f P2 = projected_origin - BIG * b1 + BIG * b2;
    Vec3f P3 = projected_origin + BIG * b1 + BIG * b2;
    Vec3f P4 = projected_origin + BIG * b1 - BIG * b2;

    glBegin(GL_QUADS);
    glNormal3f(n.x(), n.y(), n.z());
    glVertex3f(P1.x(), P1.y(), P1.z());
    glVertex3f(P2.x(), P2.y(), P2.z());
    glVertex3f(P3.x(), P3.y(), P3.z());
    glVertex3f(P4.x(), P4.y(), P4.z());
    glEnd();
}