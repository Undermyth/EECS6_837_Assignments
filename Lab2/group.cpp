#include "group.h"

Group::Group(int maxnum) {
        max_objects = maxnum;
        objs = new Object3D*[maxnum];
        num_objects = 0;
    }

Group::~Group() {
    delete[] objs;
}

bool Group::intersect(const Ray &r, Hit &h, float tmin) {
    bool intersected = false;
    for (int i = 0; i < num_objects; ++i) {
        Object3D* current = objs[i];
        if (current -> intersect(r, h, tmin)) intersected = true;
    }
    return intersected;
}

void Group::addObject(int index, Object3D *obj) {
    objs[index] = obj;
    num_objects++;
}