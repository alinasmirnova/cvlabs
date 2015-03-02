#include "mask.h"

Mask::Mask(int s)
{
    this->s = s;
    core = unique_ptr<float[]>(new float[s*s]);
}

float Mask::getPixel(int i, int j)
{
    if(i<size() && j<size() && i>=0 && j>=0)
    {
        return core[i*size() + j];
    }
    else
    {
        qFatal("getPixel: index out of the range");
        return -1;
    }
}

float Mask::setPixel(int i, int j, float value)
{
    if(i<size() && j<size() && i>=0 && j>=0)
    {
        core[i*size() + j] = value;
        return value;
    }
    else
    {
        qFatal("setPixel: index out of the range");
        return -1;
    }
}

int Mask::size()
{
    return s;
}

Mask::~Mask()
{

}

