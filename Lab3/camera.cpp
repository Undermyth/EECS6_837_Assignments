#include "camera.h"

/* OrthographicCamera */
OrthographicCamera::OrthographicCamera(Vec3f& c, Vec3f& d, Vec3f& u, int s) {
    center = c;
    direction = d;
    up = u;
    size = s;
    direction.Normalize();
    up.Normalize();
}   

Ray OrthographicCamera::generateRay(Vec2f point) {
    float x = point.x();
    float y = point.y();    // normalized coordinate

    Vec3f screenup = up - direction * up.Dot3(direction);
    screenup.Normalize();
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, screenup);
    horizontal.Normalize();

    Vec3f origin = center + (x - 0.5) * size * horizontal + (y - 0.5) * size * screenup;
    Ray ray = Ray(origin, direction);
    return ray;
}

float OrthographicCamera::getTMin() const{
    return (float)(-3e7);
}

/* functions for OpenGL */
void OrthographicCamera::glInit(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w > h)
        glOrtho(-size/2.0, size/2.0, -size*(float)h/(float)w/2.0, size*(float)h/(float)w/2.0, 0.5, 40.0);
    else
        glOrtho(-size*(float)w/(float)h/2.0, size*(float)w/(float)h/2.0, -size/2.0, size/2.0, 0.5, 40.0);
}

void OrthographicCamera::glPlaceCamera(void)
{
    gluLookAt(center.x(), center.y(), center.z(),
            center.x()+direction.x(), center.y()+direction.y(), center.z()+direction.z(),
            up.x(), up.y(), up.z());
}

void OrthographicCamera::dollyCamera(float dist)
{
    center += direction * dist;
}

void OrthographicCamera::truckCamera(float dx, float dy)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();

    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);

    center += horizontal * dx + screenUp * dy;
}

void OrthographicCamera::rotateCamera(float rx, float ry)
{
  Vec3f horizontal;
  Vec3f::Cross3(horizontal, direction, up);
  horizontal.Normalize();

  // Don't let the model flip upside-down (There is a singularity
  // at the poles when 'up' and 'direction' are aligned)
  float tiltAngle = acos(up.Dot3(direction));
  if (tiltAngle-ry > 3.13)
    ry = tiltAngle - 3.13;
  else if (tiltAngle-ry < 0.01)
    ry = tiltAngle - 0.01;

  Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
  rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

  rotMat.Transform(center);
  rotMat.TransformDirection(direction);
  
  direction.Normalize();        // may be unnecessary
}






/* PerspectiveCamera */
PerspectiveCamera::PerspectiveCamera(Vec3f& c, Vec3f& d, Vec3f& u, float g) {
    center = c;
    direction = d;
    up = u;
    angle = g;
    direction.Normalize();
    up.Normalize();
}

Ray PerspectiveCamera::generateRay(Vec2f point) {
    float x = point.x();
    float y = point.y();    // normalized coordinate

    Vec3f screenup = up - direction * up.Dot3(direction);
    screenup.Normalize();
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, screenup);
    horizontal.Normalize();
    Vec3f virtual_center = center + direction * (0.5 / tan(0.5 * angle));
    Vec3f virtual_point = virtual_center + (x - 0.5) * horizontal + (y - 0.5) * screenup;
    Vec3f ray_direction = virtual_point - center;
    ray_direction.Normalize();
    Ray ray = Ray(center, ray_direction);
    return ray;
}

float PerspectiveCamera::getTMin() const{
    return (float)(0.0);
}

/* functions for OpenGL */
void PerspectiveCamera::glInit(int w, int h)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(angle*180.0/3.14159, (float)w/float(h), 0.5, 40.0);
}

void PerspectiveCamera::glPlaceCamera(void)
{
  gluLookAt(center.x(), center.y(), center.z(),
            center.x()+direction.x(), center.y()+direction.y(), center.z()+direction.z(),
            up.x(), up.y(), up.z());
}

void PerspectiveCamera::dollyCamera(float dist)
{
  center += direction*dist;
}

void PerspectiveCamera::truckCamera(float dx, float dy)
{
  Vec3f horizontal;
  Vec3f::Cross3(horizontal, direction, up);
  horizontal.Normalize();

  Vec3f screenUp;
  Vec3f::Cross3(screenUp, horizontal, direction);

  center += horizontal*dx + screenUp*dy;
}

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
  Vec3f horizontal;
  Vec3f::Cross3(horizontal, direction, up);
  horizontal.Normalize();

  // Don't let the model flip upside-down (There is a singularity
  // at the poles when 'up' and 'direction' are aligned)
  float tiltAngle = acos(up.Dot3(direction));
  if (tiltAngle-ry > 3.13)
    ry = tiltAngle - 3.13;
  else if (tiltAngle-ry < 0.01)
    ry = tiltAngle - 0.01;

  Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
  rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

  rotMat.Transform(center);
  rotMat.TransformDirection(direction);
  direction.Normalize();
}