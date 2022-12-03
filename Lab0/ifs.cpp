#include "ifs.h"


IFS::~IFS() {
    delete[] trans;
    delete[] possible;
}

void IFS::load(const char filename[]) {
    FILE *ifs_file = fopen(filename, "r");
    assert(ifs_file != NULL);
    fscanf(ifs_file, "%d", &n);

    trans = new Matrix[n];
    possible = new float[n];

    for (int i = 0; i < n; ++i) {
        fscanf(ifs_file, "%f", &possible[i]);
        trans[i].Read3x3(ifs_file);
    }
    fclose(ifs_file);
}

void IFS::clear() {
    n = 0;
    delete[] trans;
    delete[] possible;
}

void IFS::render(Image& img, int points, int its) {
    srand((unsigned)time(NULL));
    float *prob_sum = new float[n];
    assert(prob_sum != NULL);
    for (int i = 0; i < n; ++i) prob_sum[i] = possible[i];
    for (int i = 1; i < n; ++i) prob_sum[i] += prob_sum[i - 1];

    for (int i = 0; i < points; ++i) {
        Vec2f v = Vec2f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
        for (int k = 0; k < its; ++k) {
            float p = (float)rand() / RAND_MAX;
            int renderer = 0;
            while (prob_sum[renderer] < p) {
                renderer++;
                assert(renderer < n);
            }
            trans[renderer].Transform(v);
        }
        img.SetPixel(v.x() * (img.Width() - 1), v.y() * (img.Height() - 1), Vec3f(0, 0, 0));
    }

    delete[] prob_sum;
}

