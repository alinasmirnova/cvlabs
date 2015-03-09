#include "mask.h"

Mask::Mask(int h, int w)
{
    this->height = h;
    this->width = w;
    core = make_unique<double[]>(h*w);
}

double Mask::getPixel(int i, int j) const
{
    if(i<height && j<width && i>=0 && j>=0)
    {
        return core[i*width + j];
    }
    else
    {
        qFatal("getPixel: index out of the range");
        return -1;
    }
}

double Mask::setPixel(int i, int j, double value)
{
    if(i<height && j<width && i>=0 && j>=0)
    {
        core[i*width + j] = value;
        return value;
    }
    else
    {
        qFatal("setPixel: index out of the range");
        return -1;
    }
}

int Mask::getHeight() const
{
    return height;
}
int Mask::getWidth() const
{
    return width;
}

Mask::~Mask()
{

}

