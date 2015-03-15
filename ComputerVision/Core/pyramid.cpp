#include "pyramid.h"

Pyramid::Pyramid(int octaves, int levels)
{
    octaveNum = octaves;
    levelsNum = levels;
}

bool Pyramid::saveToFolder(const QString &folderName)
{
    if(!QDir(folderName).exists())
    {
        QDir().mkdir(folderName);
    }
    QString fileName;
    bool result = true;
    for(int i=0; i<images.size(); i++)
    {
        fileName = "Octave" + QString::number(images[i]->getOctave()) + "Level" + QString::number(images[i]->getLevel()) + "Sigma" + QString::number(images[i]->getSigma());
        result = result && images[i]->getImage()->toFile(folderName + "/" + fileName + ".jpg");
    }
    return result;
}

shared_ptr<Pyramid> Pyramid::Build(const Image &image, int octaveNum, int levelNum, float sigma0, float sigmaInit)
{
    if(octaveNum > 0 && levelNum > 0)
    {
        shared_ptr<Pyramid> result = make_shared<Pyramid>(octaveNum, levelNum);

        shared_ptr<Image> next = FilterManager::SeparatedFilter(image, *MaskFactory::GaussSeparated(sqrt(sigma0*sigma0 - sigmaInit*sigmaInit)), true, EdgeMode::MIRROR);

        float k = pow(2, 1./levelNum);
        float curSigma = sigma0;
        for(int i=0; i<octaveNum; i++)
        {
            for(int j=0; j<levelNum; j++)
            {
                result->images.push_back(make_unique<PyramidLevel>(next, i, j, curSigma));
                curSigma = curSigma * k;
                next = FilterManager::SeparatedFilter(*next, *MaskFactory::GaussSeparated(k), false, EdgeMode::MIRROR);
            }
            result->images.push_back(make_unique<PyramidLevel>(next, i, levelNum, curSigma));
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

Pyramid::~Pyramid()
{

}

