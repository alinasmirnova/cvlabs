#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H
#include <memory>
#include <math.h>
#include <vector>
#include <algorithm>
#include <utility>
#include "point.h"
using namespace std;

class Descriptor
{
    vector<float> baskets;
public:
    Point point;

    Descriptor(int basketNum, Point p);

    static float distance(Descriptor d1, Descriptor d2);
    void normalize();
    void addInBasket(int basketNum, float value);

    shared_ptr<Descriptor> findClosest(vector<Descriptor> descriptors);

    ~Descriptor();
};

#endif
