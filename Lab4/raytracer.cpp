#include "raytracer.h"

RayTracer::RayTracer(SceneParser *s, int _max_bounces, float _cutoff_weight, bool _shadows) {
    scene = s;
    max_bounces = _max_bounces;
    cutoff_weight = _cutoff_weight;
    shadows = _shadows;
}

Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, 
                        float indexOfRefraction, Hit &hit) const {
    
    if (bounces > max_bounces || weight < cutoff_weight) {
        return Vec3f(0, 0, 0);
    }

    Group* objs = scene -> getGroup();
    bool intersected = objs -> intersect(ray, hit, tmin);
    if (!intersected) return scene -> getBackgroundColor();
    if (bounces == 0) RayTree::SetMainSegment(ray, 0, hit.getT());    // for debug

    /* Phong shading */
    assert (hit.getMaterial() != NULL);
    Vec3f direction, light_color;
    Vec3f object_color = hit.getMaterial() -> getDiffuseColor();
    Vec3f pixel_color = scene -> getAmbientLight() * object_color;
    Light* current_light;
    for (int l = 0; l < scene -> getNumLights(); ++l) {
        current_light = scene -> getLight(l);
        bool has_shadow = false;

        /* compute shadow */
        if (shadows) {
            Vec3f light_dir, dummy_color;
            float dist;
            current_light -> getIllumination(hit.getIntersectionPoint(), light_dir, dummy_color, dist);
            Ray shadow_ray = Ray(hit.getIntersectionPoint(), light_dir);
            Hit shadow_hit(dist, NULL, Vec3f(0, 0, 0));
            has_shadow = objs -> intersectShadowRay(shadow_ray, shadow_hit, EPI);
            RayTree::AddShadowSegment(shadow_ray, 0, shadow_hit.getT());
        }

        if (!has_shadow) {
            float dummy_dist;
            current_light -> getIllumination(hit.getIntersectionPoint(), direction, light_color, dummy_dist);
            Vec3f phong_color = hit.getMaterial() -> Shade(ray, hit, direction, light_color);
            pixel_color += phong_color;
        }
    } 

    /* tracing for reflection */
    if (hit.getMaterial() -> getReflectiveColor() != Vec3f(0, 0, 0)) {
        Vec3f normal = hit.getNormal();
        Vec3f reflect_direction = mirrorDirection(normal, ray.getDirection());
        Ray reflected_ray(hit.getIntersectionPoint(), reflect_direction);
        Hit recursive_hit(INFINITY, NULL, Vec3f(0, 0, 0));
        Vec3f reflect_color = traceRay(reflected_ray, EPI, bounces + 1, 
                                        weight * hit.getMaterial() -> getReflectiveColor().Length(),
                                        indexOfRefraction, recursive_hit);
        pixel_color += reflect_color * hit.getMaterial() -> getReflectiveColor();
        RayTree::AddReflectedSegment(reflected_ray, 0, recursive_hit.getT());
    }

    /* tracing for refraction */
    if (hit.getMaterial() -> getTransparentColor() != Vec3f(0, 0, 0)) {
        Vec3f normal = hit.getNormal();
        float index_i, index_t;
        if (normal.Dot3(ray.getDirection()) > 0) {
            normal.Negate();
            index_i = hit.getMaterial() -> getIndexOfReflection();
            index_t = 1;
        }
        else {
            index_i = 1;
            index_t = hit.getMaterial() -> getIndexOfReflection();
        }
        Vec3f refracted_dir;
        bool internal_reflect = transmittedDirection(normal, ray.getDirection(), index_i, index_t, refracted_dir);
        // if (internal_reflect) {
        //     Ray internal_reflected_ray(hit.getIntersectionPoint(), refracted_dir);
        //     Hit internal_reflect_hit(INFINITY, NULL, Vec3f(0, 0, 0));
        //     Vec3f internal_reflect_color = traceRay(internal_reflected_ray, EPI, bounces - 1,
        //                                             weight * hit.getMaterial() -> getTransparentColor().Length(),
        //                                             index_i, internal_reflect_hit);
        //     pixel_color += internal_reflect_color * hit.getMaterial() -> getTransparentColor();
        // }
        if (!internal_reflect) {
            Ray refracted_ray(hit.getIntersectionPoint(), refracted_dir);
            Hit refract_hit(INFINITY, NULL, Vec3f(0, 0, 0));
            Vec3f refract_color = traceRay(refracted_ray, EPI, bounces + 1,
                                            weight * hit.getMaterial() -> getTransparentColor().Length(),
                                            index_t, refract_hit);
            pixel_color += refract_color * hit.getMaterial() -> getTransparentColor();
            RayTree::AddTransmittedSegment(refracted_ray, 0, refract_hit.getT());
        }
    }

    return pixel_color;
}

Vec3f RayTracer::mirrorDirection(const Vec3f &normal, const Vec3f &incoming) const {
    Vec3f reflect_dir = incoming - 2 * incoming.Dot3(normal) * normal;
    reflect_dir.Normalize();
    return reflect_dir;
}

bool RayTracer::transmittedDirection(const Vec3f &normal, const Vec3f &incoming, 
                            float index_i, float index_t, Vec3f &transmitted) const {

    Vec3f N = normal;
    float eta_r = index_i / index_t;
    Vec3f I = (-1) * incoming;
    I.Normalize();
    float temp = 1 - eta_r * eta_r * (1 - N.Dot3(I) * N.Dot3(I));
    if (temp < 0) {     /* internal reflection */
        transmitted = incoming - 2 * incoming.Dot3(N) * N;
        transmitted.Normalize();
        return true;
    }
    transmitted = (eta_r * N.Dot3(I) - sqrt(temp)) * N - eta_r * I;
    transmitted.Normalize();
    return false;
}