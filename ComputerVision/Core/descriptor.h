#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H
#include <math.h>
#include <vector>
using namespace std;

class Descriptor
{
    vector<float> buckets;
public:
    Descriptor();

    static float distance(Descriptor d1, Descriptor d2);
    void normalize();
    ~Descriptor();
};

#endif // DESCRIPTOR_H
