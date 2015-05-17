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
    unique_ptr<float[]> bins;
    int binsNum;

    void simpleNormalize();
public:
    Point point;

    Descriptor(int binsNum, Point p);

    static float distance(const Descriptor& d1, const Descriptor& d2);
    void normalize();
    void addInBin(int binNum, float value);
    void addInHist(int curHistNum, float angle, float weight, int binInHist);

    pair<float,float> getMaxAngle();
    float getAngle(int bin);

    int findClosest(vector<shared_ptr<Descriptor> > descriptors);

    ~Descriptor();
};

#endif
