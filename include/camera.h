#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"
#include "vectors.h"

class Camera {

public:

    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;

};

class OrthographicCamera: public Camera {

public:

    OrthographicCamera(Vec3f& c, Vec3f& d, Vec3f& u, int s) {
        center = c;
        direction = d;
        up = u;
        size = s;
    }   

    ~OrthographicCamera() {}; 

    virtual Ray generateRay(Vec2f point) {
        float x = point.x();
        float y = point.y();    // normalized coordinate

        direction.Normalize();
        up -= direction * up.Dot3(direction);
        Vec3f horizontal;
        horizontal.Cross3(horizontal, direction, up);   // ?

        Vec3f origin = center + (x - 0.5) * size * horizontal + (y - 0.5) * size * up;
        Ray ray = Ray(origin, direction);
        return ray;
    }

    virtual float getTMin() const{
        return (float)(-3e7);
    }

private:

    Vec3f center;       // center of the camera
    Vec3f direction;    // camera direction
    Vec3f up;           // camera up direction
    int size;           // camera size

};

#endif