#include "piramida.h"

Piramida::Piramida(int octaves, int levels)
{
    octaveNum = octaves;
    levelsNum = levels;
}

bool Piramida::saveToFolder(const QString &folderName)
{
    if(!QDir(folderName).exists())
    {
        QDir().mkdir(folderName);
    }
    QString fileName;
    bool result = true;
    for(int i=0; i<images.size(); i++)
    {
        fileName = "Octave" + QString::number(i/levelsNum) + "Level" + QString::number(i%levelsNum);
        result = result && images[i]->toFile(folderName + "/" + fileName + ".jpg");
    }
    return result;
}

shared_ptr<Piramida> Piramida::Build(const Image &image, int octaveNum, int levelNum, double sigma0, double sigmaInit)
{
    if(octaveNum > 0 && levelNum > 0)
    {
        shared_ptr<Piramida> result = make_shared<Piramida>(octaveNum, levelNum);

        shared_ptr<Image> next = FilterManager::SeparatedFilter(image, *MaskFactory::GaussSeparated(sqrt(sigma0*sigma0 - sigmaInit*sigmaInit)), true, EdgeMode::MIRROR);

        double k = pow(2, 1./levelNum);
        for(int i=0; i<octaveNum; i++)
        {
            for(int j=0; j<levelNum; j++)
            {
                result->images.push_back(next);
                next = FilterManager::SeparatedFilter(*next, *MaskFactory::GaussSeparated(k), true, EdgeMode::MIRROR);
            }
            next = next->compress(2);
        }
        return result;
    }
    else
    {
        qFatal("Wrong level or octave number");
        return nullptr;
    }
}

Piramida::~Piramida()
{

}

