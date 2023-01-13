#ifndef _PLANE_H_
#define _PLANE_H_

#include "object3d.h"

#define BIG 1e5     // used for display in OpenGL

class Plane: public Object3D {

public:

    Plane(Vec3f &normal, float d_, Material *m);
    ~Plane() {};

    bool intersect(const Ray &r, Hit &h, float tmin);

    void paint(void);

private:

    Vec3f n;        // normal of the plane
    float d;        // plane equation P dot n = d

};

#endif