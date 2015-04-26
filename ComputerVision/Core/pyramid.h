#ifndef PYRAMID_H
#define PYRAMID_H
#include "image.h"
#include "filtermanager.h"
#include "maskfactory.h"
#include "pyramidlevel.h"
#include <QDir>

class Pyramid
{
    vector<shared_ptr<PyramidLevel>> images;
    vector<shared_ptr<PyramidLevel>> DoG;

public:
    int octaveNum;
    int levelsNum;

    Pyramid(int octaves, int levels);

    bool saveToFolder(const QString &folderName);

    static shared_ptr<Pyramid> build(const Image &image, int octaveNum, int levelNum, float sigma0 = 1, float sigmaInit = 0.5);
    float findPixel(int i, int j, float sigma);

    bool isLocalMaximaOrMinima(int x, int y, float scale) const;

    shared_ptr<PyramidLevel> getLevel(float sigma) const;
    shared_ptr<PyramidLevel> getLevel(int octave, int level) const;
    shared_ptr<Descriptor> getDescriptor(Point p, int surSize, int gistNum, int basketNum) const;
    ~Pyramid();
};

#endif // PYRAMID_H
