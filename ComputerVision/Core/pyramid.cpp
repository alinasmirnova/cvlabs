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
    for(uint i=0; i<images.size(); i++)
    {
        fileName = "Octave" + QString::number(images[i]->getOctave()) + "Level" + QString::number(images[i]->getLevel()) + "Sigma" + QString::number(images[i]->getSigma());
        result = result && images[i]->getImage()->toFile(folderName + "/" + fileName + ".jpg");
    }
    return result;
}

shared_ptr<Pyramid> Pyramid::build(const Image &image, int octaveNum, int levelNum, float sigma0, float sigmaInit)
{
    if(octaveNum > 0 && levelNum > 0)
    {
        shared_ptr<Pyramid> result = make_shared<Pyramid>(octaveNum, levelNum);

        result->images.push_back(make_unique<PyramidLevel>(Image::fromQImage(image.toQImage()), 0, -1, sigmaInit));

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

float Pyramid::findPixel(int i, int j, float sigma)
{
    if(sigma <= images[0]->getSigma())
    {
        return images[0]->getImage()->getPixel(i,j);
    }

    int scale;

    if(sigma >= images.back()->getSigma())
    {
        scale = pow(2,images.back()->getOctave());
        if(scale == 0) scale = 1;

        return images.back()->getImage()->getPixel(i/scale, j/scale);
    }

    int lowNum = 0;
    for(uint k=0; k<images.size() - 1; k++)
    {
        if(sigma <= images[k+1]->getSigma() && sigma >= images[k]->getSigma())
        {
            lowNum = k;
            break;
        }
    }
    int octave = images[lowNum]->getOctave();

    scale = pow(2,octave);

    float lowPixel = images[lowNum]->getImage()->getPixel(i/scale, j/scale);
    float highPixel = images[lowNum+1]->getImage()->getPixel(i/scale, j/scale);
    float lowSigma = images[lowNum]->getSigma();
    float hightSigma = images[lowNum + 1]->getSigma();
    qDebug() << "Low pixel = " << lowPixel;
    qDebug() << "High pixel = " << highPixel;
    qDebug() << "Low sigma = " << lowSigma;
    qDebug() << "High sigma = " << hightSigma;

    return lowPixel + (highPixel - lowPixel)*(sigma - lowSigma)/(hightSigma - lowSigma);
}

Pyramid::~Pyramid()
{

}

