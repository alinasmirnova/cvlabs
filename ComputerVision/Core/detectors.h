#ifndef DETECTORS_H
#define DETECTORS_H
#include "image.h"
#include <vector>
#include <algorithm>
#include <utility>
#include "filtermanager.h"
#include "maskfactory.h"

struct Point {
    int x;
    int y;
    float contrast;
public:
    Point(int x, int y, float c)
    {
        this->x = x;
        this->y = y;
        this->contrast = c;
    }
};

class Detectors
{
    static vector<Point> findLocalMax(const Image &S, int localMaxSize, float bottom);
    static float findContrast(const Image &image, int x, int y, int dx, int dy, int halfSizeW);
public:
    Detectors();

    static vector<Point> Moravec(const Image &image, int halfSizeW, int localMaxSize, float bottom);
    static vector<Point> Harris(const Image &image, int halfSizeW, int localMaxSize, float bottom);
    static vector<Point> AdaptiveNonMaximumSuppression(const vector<Point> &points, int maxPointNum, int maxR);
    ~Detectors();
};

#endif // DETECTORS_H
