#ifndef MASK_H
#define MASK_H
#include <memory>
#include <QtGlobal>
using namespace std;

class Mask
{    
public:
    Mask(int h, int w);

    float getPixel(int i, int j) const;
    float setPixel(int i, int j, float value);

    int getHeight() const;
    int getWidth() const;
    ~Mask();
private:
    unique_ptr<float[]> core;
    int height;
    int width;
};

#endif
