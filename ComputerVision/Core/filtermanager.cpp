#include "filtermanager.h"

shared_ptr<Image> FilterManager::Filter(shared_ptr<Image> source, shared_ptr<Mask> mask)
{
    shared_ptr<Image> result = make_shared<Image>(source->getHeight(), source->getWidth());

    int k = (mask->size() - 1) / 2;
    float pixel;
    int max = INT_MIN, min = INT_MAX;
    int round;

    for(int i=k; i<result->getHeight() - k; i++)
    {
        for(int j=k; j<result->getWidth() - k; j++)
        {
            pixel = 0;

            for(int u = 0; u<mask->size(); u++)
            {
                for(int v = 0; v<mask->size(); v++)
                {
                   pixel = pixel + mask->getPixel(u,v) * source->getPixel(i-(u-k), j-(v-k));
                }
            }
            round = qRound(pixel);
            result->setPixel(i, j, round);
            if(round > max)
            {
                max = round;
            }
            if(round < min)
            {
                min = round;
            }
        }
    }
    result->normalize(0,255,min,max);
    return result;
}

shared_ptr<Image> FilterManager::SobelOperator(shared_ptr<Image> source)
{
    shared_ptr<Image> gradX = FilterManager::Filter(source, MaskFactory::Sobel(Asix::X));
    shared_ptr<Image> gradY = FilterManager::Filter(source, MaskFactory::Sobel(Asix::Y));

    shared_ptr<Image> result = make_shared<Image>(source->getHeight(), source->getWidth());

    int gX, gY;
    for(int i=0; i<result->getHeight(); i++)
    {
        for(int j=0; j<result->getWidth(); j++)
        {
            gX = gradX->getPixel(i,j);
            gY = gradY->getPixel(i,j);
            result->setPixel(i,j, gX*gX + gY*gY);
        }
    }
    return result;
}

shared_ptr<Image> FilterManager::SeparatedFilter(shared_ptr<Image> source, shared_ptr<SeparatedMask> mask)
{
    shared_ptr<Image> rResult = make_shared<Image>(source->getHeight(), source->getWidth());
    shared_ptr<Image> cResult = make_shared<Image>(source->getHeight(), source->getWidth());
    float pixel;
    int round;
    int k = (mask->size() - 1) / 2;

    for(int i=0; i<rResult->getHeight(); i++)
    {
        for(int j=0; j<rResult->getWidth(); j++)
        {
            pixel = 0;
            for(int u = 0; u<mask->size(); u++)
            {
                pixel = pixel + mask->getRowPixel(u) * source->getPixel(i, j-(u-k));
            }
            rResult->setPixel(i, j, qRound(pixel));
        }
    }

    int max = INT_MIN, min = INT_MAX;
    for(int i=0; i<cResult->getHeight(); i++)
    {
        for(int j=0; j<cResult->getWidth(); j++)
        {
            pixel = 0;
            for(int u = 0; u<mask->size(); u++)
            {
                pixel = pixel + mask->getColPixel(u) * rResult->getPixel(i-(u-k), j);
            }
            round = qRound(pixel);
            cResult->setPixel(i, j, round);
            if(round > max)
            {
                max = round;
            }
            if(round < min)
            {
                min = round;
            }
        }
    }
    cResult->normalize(0,255,min,max);
    return cResult;
}
