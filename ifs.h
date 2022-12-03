#ifndef _IFS_H_
#define _IFS_H_

#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"
#include "image.h"

class IFS {

public:

    IFS() {n = 0; };
    ~IFS();

    void load(const char filename[]);
    void clear();
    void render(Image& img, int points, int its);

private:

    int n;              // the number of transformations
    Matrix *trans;      // array of transformations
    float *possible;   // probability for each transformation
};

#endif