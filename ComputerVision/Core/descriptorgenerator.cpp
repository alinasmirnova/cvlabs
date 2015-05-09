#include "descriptorgenerator.h"

DescriptorGenerator::DescriptorGenerator(const Image& image)
{
    auto dx = FilterManager::SeparatedFilter(image, *MaskFactory::SobelSeparated(Asix::X), false, EdgeMode::MIRROR);
    auto dy = FilterManager::SeparatedFilter(image, *MaskFactory::SobelSeparated(Asix::Y), false, EdgeMode::MIRROR);

    magnitudes = make_unique<Image>(image.getHeight(), image.getWidth());
    angles = make_unique<Image>(image.getHeight(), image.getWidth());

    for(int i=0; i<image.getHeight(); i++)
    {
        for(int j=0; j<image.getWidth(); j++)
        {
            angles->setPixel(i,j,atan2(dy->getPixel(i,j), dx->getPixel(i, j))*180/M_PI + 180);

            magnitudes->setPixel(i, j, sqrt(pow(dx->getPixel(i,j),2) + pow(dx->getPixel(i,j),2)));
        }
    }
}

shared_ptr<Descriptor> DescriptorGenerator::getDescriptor(Point p, int surSize, int gistNum, int beansNum)
{
    //find point angle

    auto descriptor = make_shared<Descriptor>(beansNum*gistNum, p);
    surSize = surSize*p.innerScale;

    int gistSize = ceil(surSize/gistNum);
    int curGistNum;
    float weight,angle;
    float oneBean = 360 / beansNum;
    int first, second;
    float firstValue, secondValue;

    float sigma = surSize*0.5;

    for(int i = -surSize/2; i < surSize/2; i++)
        for(int j = -surSize/2; j < surSize/2; j++)
        {
            weight = magnitudes->getPixel(p.y+i, p.x+j)*(pow(M_E,-(i*i + j*j)/(2*sigma*sigma)))/(2*M_PI*sigma*sigma);
            angle = angles->getPixel(p.y+i, p.x+j);
            first = angle/oneBean;
            if(angle < first*angle + 0.5*oneBean)
            {
                second = first - 1;
                if(second < 0) second += beansNum;
            }
            else
            {
                second = (first + 1)%beansNum;
            }
            firstValue = weight*fabs(first*angle + 0.5*oneBean - angle)/oneBean;
            secondValue = weight - firstValue;

            curGistNum = ((i + surSize/2) / gistSize) * gistNum + (j + surSize/2) / gistSize;
            descriptor->addInBean(curGistNum*beansNum + first, firstValue);
            descriptor->addInBean(curGistNum*beansNum + second, secondValue);
        }
//    int dx,dy;

//    for(int i=0; i<surSize; i++)
//    {
//        for(int j=0; j<surSize; j++)
//        {
//            curGistNum = (i/gistNum)*gistNum + (j/gistNum);
//            dx = i - surSize/2;
//            dy = j - surSize/2;

//            weight = magnitudes->getPixel(y+i, x+j)*(pow(M_E,-(dx*dx + dj*dj)/(2*sigma*sigma)))/(2*M_PI*sigma*sigma);
//            angle = angles->getPixel(y+i, x+j);
//            left = angle/oneBasket;

//            leftValue = weight*1;//(angle - left*oneBasket + oneBasket/2)/(oneBasket);

//            descriptor->addInBean(curGistNum*beansNum + left, leftValue);
//        //    descriptor->addInBean(curGistNum*beansNum + (left + 1)%beansNum, weight - leftValue);
//        }
//    }

    descriptor->normalize();
    return descriptor;
}

DescriptorGenerator::~DescriptorGenerator()
{
}

