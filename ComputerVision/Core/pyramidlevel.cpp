#include "pyramidlevel.h"

PyramidLevel::PyramidLevel(shared_ptr<Image> image, int octave, int level, float sigma)
{
    this->image = image;
    this->levelNum = level;
    this->octaveNum = octave;
    this->sigma = sigma;
}

shared_ptr<Image> PyramidLevel::getImage()
{
    return image;
}

int PyramidLevel::getLevel()
{
    return levelNum;
}

int PyramidLevel::getOctave()
{
    return octaveNum;
}

float PyramidLevel::getSigma()
{
    return sigma;
}

PyramidLevel::~PyramidLevel()
{

}

