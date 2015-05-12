#include "filtermanager.h"

shared_ptr<Image> FilterManager::Filter(const Image &source, const Mask &mask, bool norm, EdgeMode edgeMode)
{
    shared_ptr<Image> result = make_shared<Image>(source.getHeight(), source.getWidth());

    float pixel;
    int heighHalf = (mask.getHeight() - 1)/2;
    int widthHalf = (mask.getWidth() - 1)/2;

    float max = numeric_limits<float>::min(), min = numeric_limits<float>::max();

    for(int i=0; i<result->getHeight(); i++)
    {
        for(int j=0; j<result->getWidth(); j++)
        {
            pixel = 0;

            for(int u = 0; u<mask.getHeight(); u++)
            {
                for(int v = 0; v<mask.getWidth(); v++)
                {
                   pixel = pixel + mask.getPixel(u,v) * source.getPixel(i-(u-heighHalf), j-(v-widthHalf), edgeMode);
                }
            }
            result->setPixel(i, j, pixel);
            if(pixel > max)
            {
                max = pixel;
            }
            if(pixel < min)
            {
                min = pixel;
            }
        }
    }
    if(norm) result->normalize(0,255,min,max);
    return result;
}

shared_ptr<Image> FilterManager::SobelOperator(const Image &source, EdgeMode edgeMode)
{
    shared_ptr<Image> gradX = FilterManager::Filter(source, *(MaskFactory::Sobel(Asix::X)), false, edgeMode);
    shared_ptr<Image> gradY = FilterManager::Filter(source, *(MaskFactory::Sobel(Asix::Y)), false, edgeMode);

    shared_ptr<Image> result = make_shared<Image>(source.getHeight(), source.getWidth());

    float gX, gY;
    float max = numeric_limits<float>::min(), min = numeric_limits<float>::max();
    float pixel;

    for(int i=0; i<result->getHeight(); i++)
    {
        for(int j=0; j<result->getWidth(); j++)
        {
            gX = gradX->getPixel(i,j);
            gY = gradY->getPixel(i,j);
            pixel = sqrt(gX*gX + gY*gY);
            result->setPixel(i,j, pixel);
            if(pixel > max)
            {
                max = pixel;
            }
            if(pixel < min)
            {
                min = pixel;
            }
        }
    }
    result->normalize(0, 255, min, max);
    return result;
}

shared_ptr<Image> FilterManager::SeparatedFilter(const Image &source, const SeparatedMask &mask, bool norm, EdgeMode edgeMode)
{
    shared_ptr<Image> rResult = FilterManager::Filter(source, *(mask.getRow()), false, edgeMode);
    shared_ptr<Image> cResult = FilterManager::Filter(*rResult, *(mask.getColumn()), false, edgeMode);

    if(norm) cResult->normalize(0,255);
    return cResult;
}

shared_ptr<Image> FilterManager::SobelOperatorSeparated(const Image &source, EdgeMode edgeMode)
{
    shared_ptr<Image> gradX = FilterManager::SeparatedFilter(source, *(MaskFactory::SobelSeparated(Asix::X)), false, edgeMode);
    shared_ptr<Image> gradY = FilterManager::SeparatedFilter(source, *(MaskFactory::SobelSeparated(Asix::Y)), false, edgeMode);

    shared_ptr<Image> result = make_shared<Image>(source.getHeight(), source.getWidth());

    float gX, gY;
    float max = numeric_limits<float>::min(), min = numeric_limits<float>::max();
    float pixel;

    for(int i=0; i<result->getHeight(); i++)
    {
        for(int j=0; j<result->getWidth(); j++)
        {
            gX = gradX->getPixel(i,j);
            gY = gradY->getPixel(i,j);
            pixel = sqrt(gX*gX + gY*gY);
            result->setPixel(i,j, pixel);
            if(pixel > max)
            {
                max = pixel;
            }
            if(pixel < min)
            {
                min = pixel;
            }
        }
    }
    result->normalize(0, 255, min, max);
    return result;
}
