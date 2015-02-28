#include "mask.h"

Mask::Mask(int s)
{
    size = s;
    core = unique_ptr<int[]>(new int[s*s]);
}

int Mask::getPixel(int i, int j)
{
    if(i<size && j<size && i>=0 && j>=0)
    {
        return core[i*size + j];
    }
    else
    {
        qFatal("getPixel: index out of the range");
        return -1;
    }
}

int Mask::setPixel(int i, int j, int value)
{
    if(i<size && j<size && i>=0 && j>=0)
    {
        core[i*size + j] = value;
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
    return size;
}

Mask::~Mask()
{

}

