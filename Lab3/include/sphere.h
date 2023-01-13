#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "object3d.h"

extern int theta_steps, phi_steps;
extern bool gouraud;

class Sphere: public Object3D {

public:

    Sphere(Vec3f& c, float r, Material *M);

    bool intersect(const Ray &r, Hit &h, float tmin);

    void paint(void);

private:

    Vec3f center;
    float radius; 

};

#endif