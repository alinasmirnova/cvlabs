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
        shared_ptr<Image> next, top1, top2;

        float k = pow(2, 1./levelNum);
        sigma0 = sigma0 / k;
        float curSigma = sigma0;
        float bottomSigma = sigma0;
        auto octaveBottom = FilterManager::SeparatedFilter(image, *MaskFactory::GaussSeparated(sqrt(sigma0*sigma0 - sigmaInit*sigmaInit)), false, EdgeMode::MIRROR);

        for(int i=0; i<octaveNum; i++)
        {
            result->images.push_back(make_shared<PyramidLevel>(octaveBottom, i, -1, bottomSigma));
            next = octaveBottom;

            for(int j=0; j<levelNum; j++)
            {
                curSigma *= k;
                next = FilterManager::SeparatedFilter(*next, *MaskFactory::GaussSeparated(k), false, EdgeMode::MIRROR);
                result->images.push_back(make_shared<PyramidLevel>(next, i, j, curSigma));
            }

            octaveBottom = next->compress(2);
            bottomSigma = curSigma;

            top1 = FilterManager::SeparatedFilter(*next, *MaskFactory::GaussSeparated(k), false, EdgeMode::MIRROR);
            result->images.push_back(make_shared<PyramidLevel>(top1, i, levelNum, curSigma*k));

            top2 = FilterManager::SeparatedFilter(*top1, *MaskFactory::GaussSeparated(k), false, EdgeMode::MIRROR);
            result->images.push_back(make_shared<PyramidLevel>(top2, i, levelNum + 1, curSigma*k*k));
        }

        shared_ptr<Image> DoG;
        shared_ptr<Image> top, bottom;
        for(int i=0; i<result->images.size()-1; i++)
        {
            if(result->images[i]->getOctave() == result->images[i+1]->getOctave())
            {
                top = result->images[i+1]->getImage();
                bottom = result->images[i]->getImage();
                DoG = make_shared<Image>(top->getHeight(), top->getWidth());
                for(int y=0; y<DoG->getHeight(); y++)
                {
                    for(int x = 0; x<DoG->getWidth(); x++)
                    {
                        DoG->setPixel(y,x,top->getPixel(y,x) - bottom->getPixel(y,x));
                    }
                }
                result->DoG.push_back(make_shared<PyramidLevel>(DoG, result->images[i]->getOctave(), result->images[i]->getLevel(), result->images[i]->getSigma()));
            }
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

bool Pyramid::isLocalMaximaOrMinima(int x, int y, float scale) const
{
     int curX,curY;
     int maxThen;
     int minThen;
     float curValue;

     for(int i=DoG.size() - 2; i > 0; i--)
     {
         if(DoG[i]->getSigma() == scale && DoG[i+1]->getOctave() == DoG[i]->getOctave() && DoG[i]->getOctave() == DoG[i-1]->getOctave())
         {
             maxThen = minThen = 0;
             scale = pow(2, DoG[i]->getOctave());
             curX = x/scale;
             curY = y/scale;
             curValue = DoG[i]->getImage()->getPixel(curY, curX);

             for(int dx = -1; dx<=1; dx++)
             {
                 for(int dy = -1; dy<=1; dy++)
                 {
                     for(int img=-1; img<=1; img++)
                     {
                         if(curValue > DoG[i+img]->getImage()->getPixel(curY+dy, curX+dx))
                         {
                             maxThen++;
                             if(minThen > 0) return false;
                         }
                         else if(curValue < DoG[i+img]->getImage()->getPixel(curY+dy, curX+dx))
                         {
                             minThen++;
                             if(maxThen > 0) return false;
                         }
                     }
                 }
             }
             return true;
         }
     }
     return false;
}

Pyramid::~Pyramid()
{

}


shared_ptr<PyramidLevel> Pyramid::getLevel(float sigma) const
{
    for(int i=0; i<images.size(); i++)
    {
        if(sigma == images[i]->getSigma() && images[i]->getLevel() > -1 && images[i]->getLevel() < levelsNum)
        {
            return images[i];
        }
    }
    return nullptr;
}

shared_ptr<PyramidLevel> Pyramid::getLevel(int octave, int level) const
{
    for(int i=0; i<images.size(); i++)
    {
        if(octave == images[i]->getOctave() && level == images[i]->getLevel())
        {
            return images[i];
        }
    }
    return nullptr;
}
shared_ptr<Descriptor> Pyramid::getDescriptor(Point p, int surSize, int gistNum, int basketNum) const
{
    auto level = getLevel(p.scale);
    int scale = pow(2,level->getOctave());
    Point p1(p.x/scale, p.y/scale, p.contrast, p.scale);
    auto desk = level->getGenerator()->getDescriptor(p1, surSize, gistNum, basketNum);
    desk->point = p;
    return desk;
}
