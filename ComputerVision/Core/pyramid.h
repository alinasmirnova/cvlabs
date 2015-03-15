#ifndef PYRAMID_H
#define PYRAMID_H
#include "image.h"
#include "filtermanager.h"
#include "maskfactory.h"
#include "pyramidlevel.h"
#include <QDir>

class Pyramid
{
    int octaveNum;
    int levelsNum;
    vector<unique_ptr<PyramidLevel>> images;

public:
    Pyramid(int octaves, int levels);

    bool saveToFolder(const QString &folderName);

    static shared_ptr<Pyramid> Build(const Image &image, int octaveNum, int levelNum, float sigma0 = 1, float sigmaInit = 0.5);

    ~Pyramid();
};

#endif // PYRAMID_H