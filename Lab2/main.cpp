#include <string.h>

#include "camera.h"
#include "image.h"
#include "sphere.h"
#include "group.h"
#include "light.h"
#include "scene_parser.h"


int main(int argc, char* argv[]) {

    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;
    char *normal_file = NULL;
    bool shade_back = false;

    // sample command line:
    // raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc); 
            input_file = argv[i];
        } else if (!strcmp(argv[i],"-size")) {
            i++; assert (i < argc); 
            width = atoi(argv[i]);
            i++; assert (i < argc); 
            height = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-output")) {
            i++; assert (i < argc); 
            output_file = argv[i];
        } else if (!strcmp(argv[i],"-depth")) {
            i++; assert (i < argc); 
            depth_min = atof(argv[i]);
            i++; assert (i < argc); 
            depth_max = atof(argv[i]);
            i++; assert (i < argc); 
            depth_file = argv[i];
        } else if (!strcmp(argv[i],"-normals")) {
            i++; assert (i < argc);
            normal_file = argv[i];
        } else if (!strcmp(argv[i],"-shade_back")) {
            shade_back = true;
        }
        else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }

    SceneParser parser = SceneParser(input_file);
    Image img = Image(width, height);
    Image gray_img = Image(width, height);
    Image normal_img = Image(width, height);
    gray_img.SetAllPixels(Vec3f(0, 0, 0));

    Vec3f color = parser.getBackgroundColor();
    img.SetAllPixels(color);
    Group* objs = parser.getGroup();
    Camera* c = parser.getCamera();
    int light_num = parser.getNumLights();
    Vec3f ambient_color = parser.getAmbientLight();

    int size = width < height ? width : height;
    float step = 1. / size;
    Hit h;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            float x = i * step;
            float y = j * step;
            Ray r = c -> generateRay(Vec2f(x, y));
            h.set(3e7, NULL, Vec3f(0, 0, 0), Ray());
            bool intersected = objs -> intersect(r, h, c -> getTMin());
            if (intersected) {

                /* direct shading */
                // img.SetPixel(i, j, h.getMaterial()->getDiffuseColor());

                /* diffuse shading */
                assert (h.getMaterial() != NULL);
                Vec3f object_color = h.getMaterial() -> getDiffuseColor();
                Vec3f diffuse = ambient_color * object_color;
                Light* temp_light;
                Vec3f direction, light_color;
                for (int l = 0; l < light_num; ++l) {
                    temp_light = parser.getLight(l);
                    temp_light -> getIllumination(h.getIntersectionPoint(), direction, light_color);
                    float d = direction.Dot3(h.getNormal());
                    if (d < 0) {
                        if (shade_back) d = -d;     // why?
                        else d = 0;
                    }
                    diffuse += d * light_color * object_color;
                }
                img.SetPixel(i, j, diffuse);

                /* depth shading */
                if (depth_file != NULL) {
                    float depth = h.getT();
                    if (depth < depth_min) depth = depth_min;
                    else if (depth > depth_max) depth = depth_max;
                    float gray_color = 1 - (depth - depth_min) / (depth_max - depth_min);
                    gray_img.SetPixel(i, j, Vec3f(gray_color, gray_color, gray_color));
                }

                /* normal shading */
                if (normal_file != NULL) {
                    Vec3f normal_color = h.getNormal();
                    normal_img.SetPixel(i, j, Vec3f(abs(normal_color.r()), abs(normal_color.g()), abs(normal_color.b())));
                }
            }
        }
    }
    img.SaveTGA(output_file);
    if (depth_file != NULL) gray_img.SaveTGA(depth_file);
    if (normal_file != NULL) normal_img.SaveTGA(normal_file);
}

// g++ -std=c++14 -Iinclude scene_parser.cpp image.cpp main.cpp