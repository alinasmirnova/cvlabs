#ifndef DETECTORS_H
#define DETECTORS_H
#include "image.h"
#include <vector>
#include <utility>

struct Point {
    int x;
    int y;
public:
    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};

class Detectors
{
    static vector<Point> findLocalMax(const Image &S, int h, int w, int cellSize, float bottom);
    static float findContrast(const Image &image, int x, int y, int dx, int dy, int halfSizeW);
public:
    Detectors();

    static vector<Point> Moravec(const Image &image, int halfSizeW, int localMaxSize, float bottom);
    ~Detectors();
};

#endif // DETECTORS_H
