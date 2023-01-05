#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"
#include "vectors.h"

class Camera {

public:

    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
    virtual ~Camera() {}
};

class OrthographicCamera: public Camera {

public:

    OrthographicCamera(Vec3f& c, Vec3f& d, Vec3f& u, int s);
    ~OrthographicCamera() {};

    Ray generateRay(Vec2f point);
    float getTMin() const;

private:

    Vec3f center;       // center of the camera
    Vec3f direction;    // camera direction
    Vec3f up;           // camera up direction
    int size;           // camera size

};

class PerspectiveCamera: public Camera {

public:

    PerspectiveCamera(Vec3f& c, Vec3f& d, Vec3f& u, float g);
    ~PerspectiveCamera() {};

    Ray generateRay(Vec2f point);
    float getTMin() const;

private:

    Vec3f center;       // center of the camera
    Vec3f direction;    // camera direction
    Vec3f up;           // camera up direction
    float angle;        // field of perspective view

};

#endif