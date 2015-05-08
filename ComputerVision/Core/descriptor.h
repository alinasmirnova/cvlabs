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
    unique_ptr<float[]> beans;
    int beansNum;

    void simpleNormalize();
public:
    Point point;

    Descriptor(int beansNum, Point p);

    static float distance(const Descriptor& d1, const Descriptor& d2);
    void normalize();
    void addInBean(int beanNum, float value);

    tuple<int,int> getMaxBeanIndex();

    int findClosest(vector<shared_ptr<Descriptor> > descriptors);

    ~Descriptor();
};

#endif
