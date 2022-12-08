#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3d.h"

class Group: public Object3D {

public:

    Group(int maxnum) {
        max_objects = maxnum;
        objs = new Object3D*[maxnum];
        num_objects = 0;
    }

    ~Group() {
        delete[] objs;
    }

    virtual bool intersect(const Ray &r, Hit &h, float tmin) {
        h.set(3e7, NULL, Ray());
        bool intersected = false;
        for (int i = 0; i < num_objects; ++i) {
            Object3D* current = objs[i];
            if (current -> intersect(r, h, tmin)) intersected = true;
        }
        return intersected;
    }

    void addObject(int index, Object3D *obj) {
        objs[index] = obj;
        num_objects++;
    }


private:

    int max_objects;
    Object3D** objs;
    int num_objects;

};

#endif