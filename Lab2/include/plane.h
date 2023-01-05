#ifndef _PLANE_H_
#define _PLANE_H_

#include "object3d.h"

class Plane: public Object3D {

public:

    Plane(Vec3f &normal, float d_, Material *m);
    ~Plane() {};

    bool intersect(const Ray &r, Hit &h, float tmin);

private:

    Vec3f n;        // normal of the plane
    float d;        // plane equation P dot n = d

};

#endif