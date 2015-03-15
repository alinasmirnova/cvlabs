#ifndef PYRAMIDLEVEL_H
#define PYRAMIDLEVEL_H
#include "image.h"

class PyramidLevel
{
    shared_ptr<Image> image;
    int levelNum;
    int octaveNum;
    float sigma;

public:
    PyramidLevel(shared_ptr<Image> image, int octave, int level, float sigma);

    shared_ptr<Image> getImage();
    int getLevel();
    int getOctave();
    float getSigma();

    ~PyramidLevel();
};

#endif // PYRAMIDLEVEL_H
