#include "separatedmask.h"

SeparatedMask::SeparatedMask(int size)
{
    s = size;
    row = unique_ptr<float[]>(new float[size]);
    column = unique_ptr<float[]>(new float[size]);
}

float SeparatedMask::getRowPixel(int i)
{
    if(i<size() && i>=0)
    {
        return row[i];
    }
    else
    {
        qFatal("getRowPixel: index out of the range");
        return -1;
    }
}

float SeparatedMask::getColPixel(int i)
{
    if(i<size() && i>=0)
    {
        return column[i];
    }
    else
    {
        qFatal("getColPixel: index out of the range");
        return -1;
    }
}

float SeparatedMask::setRowPixel(int i, float value)
{
    if(i<size() && i>=0)
    {
        row[i] = value;
        return value;
    }
    else
    {
        qFatal("setRowPixel: index out of the range");
        return -1;
    }
}

float SeparatedMask::setColPixel(int i, float value)
{
    if(i<size() && i>=0)
    {
        column[i] = value;
        return value;
    }
    else
    {
        qFatal("setColPixel: index out of the range");
        return -1;
    }
}

int SeparatedMask::size()
{
    return s;
}

SeparatedMask::~SeparatedMask()
{

}

