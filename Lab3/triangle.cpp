#include "triangle.h"

Triangle::Triangle(Vec3f &_a, Vec3f &_b, Vec3f &_c, Material *m) {
    a = _a;
    b = _b;
    c = _c;
    Mat = m;
}

bool Triangle::intersect(const Ray &r, Hit &h, float tmin) {
    /* using barycenter method */
    Vec3f ab = a - b;
    Vec3f ac = a - c;
    Vec3f dir = r.getDirection();
    Vec3f aR = a - r.getOrigin();

    /* Cramer's rule */
    float A  = det3x3(ab.x(), ab.y(), ab.z(), ac.x(), ac.y(), ac.z(), dir.x(), dir.y(), dir.z());
    float A1 = det3x3(aR.x(), aR.y(), aR.z(), ac.x(), ac.y(), ac.z(), dir.x(), dir.y(), dir.z());
    float A2 = det3x3(ab.x(), ab.y(), ab.z(), aR.x(), aR.y(), aR.z(), dir.x(), dir.y(), dir.z());
    float A3 = det3x3(ab.x(), ab.y(), ab.z(), ac.x(), ac.y(), ac.z(), aR.x(), aR.y(), aR.z());

    float beta = A1 / A;
    float gamma = A2 / A;
    float t = A3 / A;

    if (beta + gamma < 1 && beta > 0 && gamma > 0) {
        if (t > tmin && t < h.getT()) {
            Vec3f normal;
            Vec3f::Cross3(normal, ab, ac);      // direction to be modified
            normal.Normalize();
            h.set(t, Mat, normal, r);
            return true;
        }
        else return false;
    }
    else return false;
}

void Triangle::paint(void) {
    Mat -> glSetMaterial();
    Vec3f ab = a - b;
    Vec3f ac = a - c;
    Vec3f normal;
    Vec3f::Cross3(normal, ab, ac);
    glBegin(GL_TRIANGLES);
    glNormal3f(normal.x(), normal.y(), normal.z());
    glVertex3f(a.x(), a.y(), a.z());
    glVertex3f(b.x(), b.y(), b.z());
    glVertex3f(c.x(), c.y(), c.z());
    glEnd();
}