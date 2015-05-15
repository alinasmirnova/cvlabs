#ifndef MODELS_H
#define MODELS_H
#include "image.h"
#include "descriptor.h"
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>

class Models
{
    vector<pair<shared_ptr<Descriptor>, shared_ptr<Descriptor>>> matches;
public:
    vector<pair<Point, Point>> best;

    Models(vector<shared_ptr<Descriptor>> desc1, vector<shared_ptr<Descriptor>> desc2);

    float *RanSaC(int iterCount, float eps);

    ~Models();
};

#endif
