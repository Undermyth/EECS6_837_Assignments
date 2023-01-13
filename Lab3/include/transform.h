#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "object3d.h"
#include "matrix.h"

class Transform: public Object3D {

public:

    Transform(Matrix &m, Object3D *o);
    ~Transform() {};

    bool intersect(const Ray &r, Hit &h, float tmin);

    void paint(void);

private:

    Matrix M;
    Object3D* obj;
    Matrix M_inversed;
    Matrix M_inversed_T;
};

#endif