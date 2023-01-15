#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3d.h"

class Group: public Object3D {

public:

    Group(int maxnum);
    ~Group();

    bool intersect(const Ray &r, Hit &h, float tmin);
    bool intersectShadowRay(const Ray &r, Hit &h, float tmin);
    void addObject(int index, Object3D *obj);

    void paint(void);
    
private:

    int max_objects;
    Object3D** objs;
    int num_objects;

};

#endif