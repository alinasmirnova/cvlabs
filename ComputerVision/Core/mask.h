#ifndef MASK_H
#define MASK_H
#include <memory>
#include <QtGlobal>
using namespace std;

class Mask
{    
public:
    Mask(int h, int w);

    float getPixel(int i, int j);
    float setPixel(int i, int j, float value);

    inline int getHeight();
    inline int getWidth();
    ~Mask();
private:
    unique_ptr<float[]> core;
    int height;
    int width;
};

#endif // MASK_H
