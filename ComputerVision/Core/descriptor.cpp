#include "descriptor.h"

Descriptor::Descriptor()
{

}

float Descriptor::distance(Descriptor d1, Descriptor d2)
{
    float distance = 0;

    for(int i=0; i<d1.buckets.size(); i++)
    {
        distance += pow(d1.buckets[i] - d2.buckets[i], 2);
    }

    return sqrt(distance);
}

void Descriptor::normalize()
{
    float min = 2;
    float max = 0;
    for(int i=0; i<buckets.size(); i++)
    {
        if(buckets[i] > 2)
        {
            buckets[i] = 2;
        }
        else if(buckets[i] < 0)
        {
            buckets[i] = 0;
        }

        if(buckets[i] < min)
        {
            min = buckets[i];
        }
        if(buckets[i] > max)
        {
            max = buckets[i];
        }
    }

    for(int i=0; i<buckets.size(); i++)
    {
        buckets[i] = min + (max - min)*buckets[i];
    }
}

Descriptor::~Descriptor()
{

}

