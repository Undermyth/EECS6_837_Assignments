#include <string.h>

#include <GL/glut.h>

#include "camera.h"
#include "image.h"
#include "sphere.h"
#include "group.h"
#include "light.h"
#include "scene_parser.h"
#include "glCanvas.h"
#include "raytracer.h"

char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char *depth_file = NULL;
char *normal_file = NULL;
bool shade_back = false;
bool gui = false;
int theta_steps, phi_steps;
bool gouraud = false;
bool shadows = false;
int bounces = 1;
float weight = 0.01;

void glRayTracer(float x, float y) {
    SceneParser parser = SceneParser(input_file);
    Camera* c = parser.getCamera();
    RayTracer tracer(&parser, bounces, weight, shadows);

    int size = width < height ? width : height;
    float step = 1. / size;
    Hit h;
    Ray r = c -> generateRay(Vec2f(x, y));
    h.set(INFINITY, NULL, Vec3f(0, 0, 0), Ray());
    tracer.traceRay(r, c -> getTMin(), 0, 1, 1, h);
}

void render(void) {
    SceneParser parser = SceneParser(input_file);
    Image img = Image(width, height);
    Image gray_img = Image(width, height);
    Image normal_img = Image(width, height);

    Vec3f color = parser.getBackgroundColor();
    Group* objs = parser.getGroup();
    Camera* c = parser.getCamera();
    gray_img.SetAllPixels(Vec3f(0, 0, 0));
    img.SetAllPixels(color);

    RayTracer tracer(&parser, bounces, weight, shadows);

    int size = width < height ? width : height;
    float step = 1. / size;
    Hit h;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            float x = i * step;
            float y = j * step;
            Ray r = c -> generateRay(Vec2f(x, y));
            h.set(INFINITY, NULL, Vec3f(0, 0, 0), Ray());
            Vec3f pixel_color = tracer.traceRay(r, c -> getTMin(), 0, 1, 1, h); /*!!*/
            img.SetPixel(i, j, pixel_color);

            /* reserved for depth shading and normal shading. can be removed? */
            h.set(INFINITY, NULL, Vec3f(0, 0, 0), Ray());
            bool intersected = objs -> intersect(r, h, c -> getTMin());
            if (intersected) {

                /* depth shading */
                if (depth_file != NULL) {
                    float depth = h.getT();
                    if (depth < depth_min) depth = depth_min;
                    else if (depth > depth_max) depth = depth_max;
                    float gray_color = 1 - (depth - depth_min) / (depth_max - depth_min);
                    gray_img.SetPixel(i, j, Vec3f(gray_color, gray_color, gray_color));
                }

                /* normal visualization */
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

int main(int argc, char* argv[]) {

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
        } else if (!strcmp(argv[i],"-gui")) {
            gui = true;
        } else if (!strcmp(argv[i], "-tessellation")) {
            i++; assert(i < argc);
            theta_steps = atoi(argv[i]);
            i++; assert(i < argc);
            phi_steps = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-gouraud")) {
            gouraud = true;
        } else if (!strcmp(argv[i], "-shadows")) {
            shadows = true;
        } else if (!strcmp(argv[i], "-bounces")) {
            i++; assert(i < argc);
            bounces = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-weight")) {
            i++; assert(i < argc);
            weight = atoi(argv[i]);
        }
        else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }

    SceneParser parser = SceneParser(input_file);
    if (gui) {
        GLCanvas canvas;
        glutInit(&argc, argv);
        canvas.initialize(&parser, render, glRayTracer);
    }
    else {
        render();
    }
}