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

void Sphere::paint(void) {
    Mat -> glSetMaterial();
    float theta_per_step = 2 * M_PI / theta_steps;
    float phi_per_step = M_PI / phi_steps;
    float phi = -M_PI / 2;
    float theta = 0;
    glBegin(GL_QUADS);
    for (int iPhi = 0; iPhi < phi_steps; iPhi++) {
        theta = 0;
        for (int iTheta = 0; iTheta < theta_steps; iTheta++) {

            Vec3f P[] = {
                Vec3f(cos(phi) * sin(theta), sin(phi), cos(phi) * cos(theta)),
                Vec3f(cos(phi + phi_per_step) * sin(theta), sin(phi + phi_per_step), cos(phi + phi_per_step) * cos(theta)),
                Vec3f(cos(phi + phi_per_step) * sin(theta + theta_per_step), sin(phi + phi_per_step), cos(phi + phi_per_step) * cos(theta + theta_per_step)),
                Vec3f(cos(phi) * sin(theta + theta_per_step), sin(phi), cos(phi) * cos(theta + theta_per_step))
            };
            
            if (!gouraud) {     // necessary ?
                Vec3f normal;
                Vec3f::Cross3(normal, P[3] - P[0], P[1] - P[0]);
                normal.Normalize();
                glNormal3f(normal.x(), normal.y(), normal.z());
            }
            for (int i = 0; i < 4; i++) {
                if (gouraud) {
                    glNormal3f(P[i].x(), P[i].y(), P[i].z());
                }
                P[i] = center + radius * P[i];
                glVertex3f(P[i].x(), P[i].y(), P[i].z());
            }
            theta += theta_per_step;
        }
        phi += phi_per_step;
    }
    glEnd();
}