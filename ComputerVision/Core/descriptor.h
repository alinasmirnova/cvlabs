#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H
#include <memory>
#include <math.h>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

class Descriptor
{
    vector<float> baskets;
public:
    int x, y;

    Descriptor(int basketNum, int x, int y);

    static float distance(Descriptor d1, Descriptor d2);
    void normalize();
    void addInBasket(int basketNum, float value);

    shared_ptr<Descriptor> findClosest(vector<Descriptor> descriptors);

    ~Descriptor();
};

#endif
