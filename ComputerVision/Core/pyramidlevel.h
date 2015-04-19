#ifndef PYRAMIDLEVEL_H
#define PYRAMIDLEVEL_H
#include "image.h"
#include "descriptorgenerator.h"
class PyramidLevel
{
    shared_ptr<Image> image;
    int levelNum;
    int octaveNum;
    float sigma;

    shared_ptr<DescriptorGenerator> generator;
public:
    PyramidLevel(shared_ptr<Image> image, int octave, int level, float sigma);

    shared_ptr<Image> getImage();
    shared_ptr<DescriptorGenerator> getGenerator();
    int getLevel();
    int getOctave();
    float getSigma();

    ~PyramidLevel();
};

#endif // PYRAMIDLEVEL_H
