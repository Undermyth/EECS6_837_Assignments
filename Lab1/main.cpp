#include <string.h>

#include "camera.h"
#include "image.h"
#include "sphere.h"
#include "group.h"
#include "scene_parser.h"


int main(int argc, char* argv[]) {

    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;

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
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }

    SceneParser parser = SceneParser(input_file);
    Image img = Image(width, height);
    Image gray_img = Image(width, height);
    gray_img.SetAllPixels(Vec3f(0, 0, 0));

    Vec3f color = parser.getBackgroundColor();
    img.SetAllPixels(color);
    Group* objs = parser.getGroup();
    Camera* c = parser.getCamera();

    int size = width < height ? width : height;
    float step = 1. / size;
    Hit h;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            float x = i * step;
            float y = j * step;
            Ray r = c -> generateRay(Vec2f(x, y));
            bool intersected = objs -> intersect(r, h, c -> getTMin());
            if (intersected) {
                img.SetPixel(i, j, h.getMaterial()->getDiffuseColor());
                float depth = h.getT();
                if (depth < depth_min) depth = depth_min;
                else if (depth > depth_max) depth = depth_max;
                float gray_color = 1 - (depth - depth_min) / (depth_max - depth_min);
                gray_img.SetPixel(i, j, Vec3f(gray_color, gray_color, gray_color));
            }
        }
    }
    img.SaveTGA(output_file);
    gray_img.SaveTGA(depth_file);
}

// g++ -std=c++14 -Iinclude scene_parser.cpp image.cpp main.cpp