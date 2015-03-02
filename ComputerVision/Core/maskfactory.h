#ifndef MASKFACTORY_H
#define MASKFACTORY_H
#include "mask.h"
#include "separatedmask.h"
#define M_PI 3.14159265358979323846
#define M_E 2.71828182845905

enum Direction{
    LEFT = 0,
    RIGHT = 1,
    UP = 2,
    DOWN = 3
};

enum Asix{
    X = 0,
    Y = 1
};

class MaskFactory
{
public:
    MaskFactory();

    static shared_ptr<Mask> Shift(int pixelNum, Direction direction);
    static shared_ptr<Mask> Blur(int k, int blur);
    static shared_ptr<Mask> Sobel(Asix asix);
    static shared_ptr<Mask> Pruit(Asix asix);
    static shared_ptr<Mask> Shar(Asix asix);
    static shared_ptr<Mask> Gauss(int k, int theta);

    static shared_ptr<SeparatedMask> GaussSeparated(int k, int theta);
    static shared_ptr<SeparatedMask> SobelSeparated(Asix asix);

    ~MaskFactory();
};

#endif // MASKFACTORY_H
