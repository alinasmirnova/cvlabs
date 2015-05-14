#ifndef DETECTORS_H
#define DETECTORS_H
#include "image.h"
#include <vector>
#include <algorithm>
#include <utility>
#include "filtermanager.h"
#include "maskfactory.h"
#include "pyramid.h"
#include "point.h"

class Detectors
{
    static vector<Point> findLocalMax(const Image &S, int localMaxSize, int halfSizeW, float bottom);
    static float findContrast(const Image &image, int x, int y, int dx, int dy, int halfSizeW);
    static float getDistance(Point p1, Point p2);
public:
    Detectors();

    static vector<Point> Moravec(const Image &image, int halfSizeW, int localMaxSize, float bottom);
    static vector<Point> Harris(const Image &image, int halfSizeW, int localMaxSize, float bottom);

    static vector<Point> ScaleInvariant(const Pyramid& pyramid);

    static vector<Point> AdaptiveNonMaximumSuppression(const vector<Point> &points, int maxPointNum, int maxR);
    ~Detectors();
};

#endif // DETECTORS_H
