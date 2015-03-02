#ifndef MASK_H
#define MASK_H
#include <memory>
#include <QtGlobal>
using namespace std;

class Mask
{    
public:
    Mask(int s);

    float getPixel(int i, int j);
    float setPixel(int i, int j, float value);

    int size();
    ~Mask();
private:
    unique_ptr<float[]> core;
    int s;
};

#endif // MASK_H
