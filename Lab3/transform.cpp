#include "transform.h"

Transform::Transform(Matrix &m, Object3D *o) {
    M = m;
    obj = o;
    M.Inverse(M_inversed);
    M_inversed.Transpose(M_inversed_T);
}

bool Transform::intersect(const Ray &r, Hit &h, float tmin) {

    /* Transform the ray */
    Vec3f origin_os = r.getOrigin();
    Vec3f dir_os = r.getDirection();
    M_inversed.Transform(origin_os);
    M_inversed.TransformDirection(dir_os);
    // float amplify_t = dir_os.Length();
    // dir_os.Normalize();
    Ray r_os = Ray(origin_os, dir_os);

    /* do Intersect in the object space */
    bool intersected = obj -> intersect(r_os, h, tmin);
    if (intersected) {
        float t_ws = h.getT();
        Material* m_tmp = h.getMaterial();
        Vec3f normal_ws = h.getNormal();
        // t_ws /= amplify_t;
        M_inversed_T.TransformDirection(normal_ws);
        normal_ws.Normalize();
        h.set(t_ws, m_tmp, normal_ws, r);
        return true;
    }
    else return false;
}

void Transform::paint(void) {
    // Mat -> glSetMaterial();
    glPushMatrix();
    GLfloat *glMatrix = M.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;
    obj -> paint();
    glPopMatrix();
}