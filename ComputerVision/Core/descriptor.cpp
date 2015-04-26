#include "descriptor.h"

Descriptor::Descriptor(int beansNum, Point p): beansNum(beansNum), point(p)
{
    beans = make_unique<float[]>(beansNum);
}

float Descriptor::distance(const Descriptor& d1, const Descriptor& d2)
{
    float distance = 0;

    for(int i=0; i<d1.beansNum; i++)
    {
        distance += pow(d1.beans[i] - d2.beans[i], 2);
    }

    return sqrt(distance);
}

void Descriptor::simpleNormalize()
{
    auto m = minmax_element(&beans[0], &beans[beansNum]);;
    float minValue = *m.first;
    float maxValue = *m.second;

    for(int i=0; i<beansNum; i++)
    {
        beans[i] = (beans[i] - minValue)/(maxValue - minValue);
    }
}

void Descriptor::normalize()
{
    simpleNormalize();
    for(int i = 0; i<beansNum; i++)
    {
        beans[i] = min(beans[i], 0.2f);
    }
    simpleNormalize();
}

void Descriptor::addInBean(int beanNum, float value)
{
    if(beanNum >= 0 && beanNum < beansNum)
    {
        beans[beanNum] += value;
    }
}

int Descriptor::findClosest(vector<shared_ptr<Descriptor>> descriptors)
{
    int minIndex1 = -1, minIndex2 = -1;
    float minValue1 = numeric_limits<float>::max(), minValue2 = numeric_limits<float>::max(), value;

    for(int i=0; i<descriptors.size(); i++)
    {
        value = distance(*this, *descriptors[i]);
        if(value < minValue1)
        {
            minValue2 = minValue1;
            minIndex2 = minIndex1;

            minValue1 = value;
            minIndex1 = i;
        }
        else if(value < minValue2)
        {
            minValue2 = value;
            minIndex2 = i;
        }
    }
    if(minValue1 / minValue2 < 0.8)
    {
        return minIndex1;
    }
    return -1;
}

Descriptor::~Descriptor()
{

}

