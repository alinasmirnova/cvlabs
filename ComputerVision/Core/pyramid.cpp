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
    for(uint i=0; i<DoG.size(); i++)
    {
        fileName = "Octave" + QString::number(DoG[i]->getOctave()) + "Level" + QString::number(DoG[i]->getLevel()) + "Sigma" + QString::number(DoG[i]->getSigma());
        //result = result && images[i]->getImage()->toFile(folderName + "/" + fileName + ".jpg");
        result = result && DoG[i]->getImage()->toFile(folderName + "/D" + fileName + ".jpg");
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
        curSigma *= k;

        float innerSigma = 1;


        for(int i=0; i<octaveNum; i++)
        {
            innerSigma = 1;
            result->images.push_back(make_shared<PyramidLevel>(octaveBottom, i, -1, bottomSigma, innerSigma));
            next = octaveBottom;
            curSigma = bottomSigma;

            for(int j=0; j<levelNum; j++)
            {
                next = FilterManager::SeparatedFilter(*next, *MaskFactory::GaussSeparated(curSigma*sqrt(k*k-1)), false, EdgeMode::MIRROR);
                curSigma *= k;
                innerSigma *= k;
                result->images.push_back(make_shared<PyramidLevel>(next, i, j, curSigma, innerSigma));
            }

            octaveBottom = next->compress(2);
            bottomSigma = curSigma;


            top1 = FilterManager::SeparatedFilter(*next, *MaskFactory::GaussSeparated(curSigma*sqrt(k*k-1)), false, EdgeMode::MIRROR);
            curSigma *= k;
            innerSigma *= k;
            result->images.push_back(make_shared<PyramidLevel>(top1, i, levelNum, curSigma, innerSigma));

            top2 = FilterManager::SeparatedFilter(*top1, *MaskFactory::GaussSeparated(curSigma*sqrt(k*k-1)), false, EdgeMode::MIRROR);
            curSigma *= k;
            innerSigma *= k;
            result->images.push_back(make_shared<PyramidLevel>(top2, i, levelNum + 1, curSigma, innerSigma));
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
                result->DoG.push_back(make_shared<PyramidLevel>(DoG, result->images[i]->getOctave(), result->images[i]->getLevel(), result->images[i]->getSigma(), result->images[i]->getInnerSigma()));
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

//координаты в оригинальном изображении. ищем для масштаба sigma
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

vector<Point> Pyramid::findLocalMaximaAndMinima(int halfWindow) const
{
    vector<Point> result;

    float curValue, value;
    bool isLocalMaxMin;
    int more, less;
    int octaveNum, curScale;
    int border;
    float trace, det;

    shared_ptr<Image> dxx, dyy, dxy;

    for(int dog = 1; dog < DoG.size() - 1; dog++)
    {
        if(DoG[dog-1]->getOctave() != DoG[dog]->getOctave() || DoG[dog+1]->getOctave() != DoG[dog]->getOctave())
            continue;

        dxx = FilterManager::SeparatedFilter(*DoG[dog]->getImage(), *MaskFactory::SobelSeparated(Asix::X));
        dyy = FilterManager::SeparatedFilter(*DoG[dog]->getImage(), *MaskFactory::SobelSeparated(Asix::Y));

        dxy = FilterManager::SeparatedFilter(*dxx, *MaskFactory::SobelSeparated(Asix::Y));
        dyy = FilterManager::SeparatedFilter(*dyy, *MaskFactory::SobelSeparated(Asix::Y));
        dxx = FilterManager::SeparatedFilter(*dxx, *MaskFactory::SobelSeparated(Asix::X));


        border = halfWindow*DoG[dog]->getSigma();

        for(int x = border; x < DoG[dog]->getImage()->getWidth()-border; x++)
        {
            for(int y = border; y < DoG[dog]->getImage()->getHeight()-border; y++)
            {                
                curValue = DoG[dog]->getImage()->getPixel(y, x);
                if(fabs(curValue) < 6) continue;

                isLocalMaxMin = true;
                more = 0;
                less = 0;
                for(int dNum = -1; dNum <= 1; dNum++)
                {
                    for(int dx = -1; dx <= 1; dx++)
                    {
                        for(int dy = -1; dy <= 1; dy++)
                        {
                            if(dNum == 0 && dx == 0 && dy == 0) continue;

                            value = DoG[dog + dNum]->getImage()->getPixel(y + dy, x + dx);
                            if(curValue > value)
                            {
                                more++;
                                if(less > 0)
                                {
                                    isLocalMaxMin = false;
                                    goto exit;
                                }
                            }
                            else if(curValue < value)
                            {
                                less++;
                                if(more > 0)
                                {
                                    isLocalMaxMin = false;
                                    goto exit;
                                }
                            }
                        }
                    }
                }
                exit: if(isLocalMaxMin)
                {
                    curScale = pow(2,DoG[dog]->getOctave());

                    //удаление точек на краях
                    trace = dxx->getPixel(y,x) + dyy->getPixel(y,x);
                    det = dxx->getPixel(y,x) * dyy->getPixel(y,x) - pow(dxy->getPixel(y,x),2);

                    if(pow(trace,2) / det <= pow(11,2)/10)
                    {
                        result.push_back(Point(x*curScale, y*curScale, curValue, DoG[dog]->getInnerSigma()));
                    }
                }
            }
        }
    }
    return result;
}


vector<Point> Pyramid::isLocalMaximaAndMinima(int x, int y) const
{
    vector<Point> result;

    float curValue, value;
    bool isLocalMaxMin;
    int more, less;
    int octaveNum, curScale;

    int curX, curY;

    for(int dog = 1; dog < DoG.size() - 1; dog++)
    {
        if(DoG[dog-1]->getOctave() != DoG[dog]->getOctave() || DoG[dog+1]->getOctave() != DoG[dog]->getOctave())
            continue;

        curX = x/pow(2, DoG[dog]->getOctave());
        curY = y/pow(2, DoG[dog]->getOctave());

        if(fabs(curX - DoG[dog]->getImage()->getWidth()) < 16*DoG[dog]->getSigma() ||
           fabs(curY - DoG[dog]->getImage()->getHeight()) < 16*DoG[dog]->getSigma())
            continue;

        curValue = DoG[dog]->getImage()->getPixel(curY, curX);
        if(fabs(curValue) < 3.65) continue;

        isLocalMaxMin = true;
        more = 0;
        less = 0;
        for(int dNum = -1; dNum <= 1; dNum++)
        {
            for(int dx = -1; dx <= 1; dx++)
            {
                for(int dy = -1; dy <= 1; dy++)
                {
                    if(dNum == 0 && dx == 0 && dy == 0) continue;

                    value = DoG[dog + dNum]->getImage()->getPixel(curY + dy, curX + dx);
                    if(curValue > value)
                    {
                        more++;
                        if(less > 0)
                        {
                            isLocalMaxMin = false;
                            goto exit;
                        }
                    }
                    else if(curValue < value)
                    {
                        less++;
                        if(more > 0)
                        {
                            isLocalMaxMin = false;
                            goto exit;
                        }
                    }
                }
            }
        }

        exit: if(isLocalMaxMin)
        {
            result.push_back(Point(x, y, curValue, DoG[dog]->getSigma()));
        }

    }
    return result;
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
    //find point angle
    auto gist = level->getGenerator()->getDescriptor(p1, surSize, 1, 36);

    auto desk = level->getGenerator()->getDescriptor(p1, surSize, gistNum, basketNum);
    desk->point = p;
    return desk;
}
