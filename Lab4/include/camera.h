#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <GL/gl.h>
#include <GL/glu.h>

#include "ray.h"
#include "vectors.h"
#include "matrix.h"

class Camera {

public:

    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
    virtual ~Camera() {}

    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    virtual void dollyCamera(float dist) = 0;
    virtual void truckCamera(float dx, float dy) = 0;
    virtual void rotateCamera(float rx, float ry) = 0;
};

class OrthographicCamera: public Camera {

public:

    OrthographicCamera(Vec3f& c, Vec3f& d, Vec3f& u, int s);
    ~OrthographicCamera() {};

    Ray generateRay(Vec2f point);
    float getTMin() const;

    /* Added function for OpenGL */
    void glInit(int w, int h);
    void glPlaceCamera(void);
    void dollyCamera(float dist);
    void truckCamera(float dx, float dy);
    void rotateCamera(float rx, float ry);

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

    /* Added function for OpenGL */
    void glInit(int w, int h);
    void glPlaceCamera(void);
    void dollyCamera(float dist);
    void truckCamera(float dx, float dy);
    void rotateCamera(float rx, float ry);

private:

    Vec3f center;       // center of the camera
    Vec3f direction;    // camera direction
    Vec3f up;           // camera up direction
    float angle;        // field of perspective view

};

#endif