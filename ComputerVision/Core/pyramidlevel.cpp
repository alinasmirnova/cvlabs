#include "pyramidlevel.h"

PyramidLevel::PyramidLevel(shared_ptr<Image> image, int octave, int level, float sigma, float innerSigma)
{
    this->image = image;
    this->levelNum = level;
    this->octaveNum = octave;
    this->sigma = sigma;
    this->innerSigma = sigma;
    this->generator = make_shared<DescriptorGenerator>(*image);
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

float PyramidLevel::getInnerSigma()
{
    return innerSigma;
}

shared_ptr<DescriptorGenerator> PyramidLevel::getGenerator()
{
    return generator;
}

PyramidLevel::~PyramidLevel()
{

}

