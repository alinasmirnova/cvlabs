#ifndef MASK_H
#define MASK_H
#include <memory>
#include <QtGlobal>
using namespace std;

class Mask
{    
public:
    Mask(int h, int w);

    double getPixel(int i, int j) const;
    double setPixel(int i, int j, double value);

    int getHeight() const;
    int getWidth() const;
    ~Mask();
private:
    unique_ptr<double[]> core;
    int height;
    int width;
};

#endif
