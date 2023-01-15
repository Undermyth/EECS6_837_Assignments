#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

#include "scene_parser.h"
#include "ray.h"
#include "hit.h"
#include "group.h"
#include "light.h"
#include "rayTree.h"

#define EPI 1e-2    /* small constant for self shadowing */

class RayTracer {

public:

    RayTracer(SceneParser *s, int _max_bounces, float _cutoff_weight, bool _shadows);
    ~RayTracer() {};

    Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, 
                float indexOfRefraction, Hit &hit) const;

    Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming) const;
    bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming, 
                            float index_i, float index_t, Vec3f &transmitted) const;

private:

    SceneParser *scene;
    int max_bounces;
    float cutoff_weight;
    bool shadows;

};

#endif