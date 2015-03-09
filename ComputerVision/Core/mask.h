#ifndef MASK_H
#define MASK_H
#include <memory>
#include <QtGlobal>
using namespace std;

class Mask
{    
public:
    Mask(int h, int w);

    double getPixel(int i, int j);
    double setPixel(int i, int j, double value);

    inline int getHeight();
    inline int getWidth();
    ~Mask();
private:
    unique_ptr<double[]> core;
    int height;
    int width;
};

#endif
