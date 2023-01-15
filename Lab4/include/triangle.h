#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object3d.h"
#include "matrix.h"

#define TRIANGLE_EPI 1e-3

class Triangle: public Object3D {

public:

    Triangle(Vec3f &_a, Vec3f &_b, Vec3f &_c, Material *m);
    ~Triangle() {};

    bool intersect(const Ray &r, Hit &h, float tmin);
    bool intersectShadowRay(const Ray &r, Hit &h, float tmin);

    void paint(void);

private:

    Vec3f a;
    Vec3f b;
    Vec3f c;

};

#endif