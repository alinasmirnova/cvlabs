#ifndef PIRAMIDA_H
#define PIRAMIDA_H
#include "image.h"
#include "filtermanager.h"
#include "maskfactory.h"
#include <QDir>
class Piramida
{
    int octaveNum;
    int levelsNum;
    vector<shared_ptr<Image>> images;

public:
    Piramida(int octaves, int levels);

    bool saveToFolder(const QString &folderName);

    static shared_ptr<Piramida> Build(const Image &image, int octaveNum, int levelNum, double sigma0 = 1, double sigmaInit = 0.5);

    ~Piramida();
};

#endif // PIRAMIDA_H
