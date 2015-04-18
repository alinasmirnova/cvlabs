#include "descriptor.h"

Descriptor::Descriptor(int basketNum, int x, int y)
{
    for(int i=0; i<basketNum; i++)
    {
        baskets.push_back(0);
    }
    this->x = x;
    this->y = y;
}

float Descriptor::distance(Descriptor d1, Descriptor d2)
{
    float distance = 0;

    for(int i=0; i<d1.baskets.size(); i++)
    {
        distance += pow(d1.baskets[i] - d2.baskets[i], 2);
    }

    return sqrt(distance);
}

void Descriptor::normalize()
{
    float minValue = numeric_limits<float>::max();
    float maxValue = 0;
    for(int i=0; i<baskets.size(); i++)
    {
        //baskets[i] = max(0.0f, min(2.0f, baskets[i]));
        minValue = min(minValue, baskets[i]);
        maxValue = max(maxValue, baskets[i]);
    }

    for(int i=0; i<baskets.size(); i++)
    {
        baskets[i] = (baskets[i] - minValue)/(maxValue - minValue);
    }
}

void Descriptor::addInBasket(int basketNum, float value)
{
    if(basketNum > 0 && basketNum < (int)baskets.size())
    {
        baskets[basketNum] += value;
    }
}

shared_ptr<Descriptor> Descriptor::findClosest(vector<shared_ptr<Descriptor>> descriptors)
{
    sort(begin(descriptors), end(descriptors), [this](auto a, auto b) {return distance(*this,*a) < distance(*this,*b);});

    float d1 = distance(*this, *descriptors[0]);
    float d2 = distance(*this, *descriptors[1]);

    if(distance(*this, *descriptors[0]) / distance(*this, *descriptors[1]) < 0.8)
    {
        return descriptors[0];
    }
    return nullptr;
}

Descriptor::~Descriptor()
{

}

