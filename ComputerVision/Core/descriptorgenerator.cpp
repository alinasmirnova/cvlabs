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

shared_ptr<Descriptor> DescriptorGenerator::getAngleDescriptor(Point p, int surSize)
{
    int beansNum = 48;
    int gistNum = 1;

    auto descriptor = make_shared<Descriptor>(beansNum*gistNum, p);
    surSize = surSize*p.innerScale;

    int gistSize = ceil(surSize/gistNum);
    int curGistNum;
    float weight,angle,r;
    float oneBean = 360 / beansNum;
    int first, second;
    float firstValue, secondValue;

    float sigma = surSize*0.5;

    for(int i = -surSize/2; i < surSize/2; i++)
        for(int j = -surSize/2; j < surSize/2; j++)
        {
            weight = magnitudes->getPixel(p.y+i, p.x+j)*(pow(M_E,-(i*i + j*j)/(2*sigma*sigma)))/(2*M_PI*sigma*sigma);
            angle = angles->getPixel(p.y+i, p.x+j);

            curGistNum = ((i + surSize/2) / gistSize) * gistNum + (j + surSize/2) / gistSize;

            descriptor->addInGist(curGistNum, angle, weight, beansNum);
        }
    return descriptor;
}


shared_ptr<Descriptor> DescriptorGenerator::getDescriptor(Point p, int surSize, int gistNum, int beansNum)
{
    //find point angle

    auto descriptor = make_shared<Descriptor>(beansNum*gistNum, p);
    surSize = surSize*p.innerScale;

    int gistSize = ceil(surSize/gistNum);
    int curGistNum;
    float weight,angle,r;
    float oneBean = 360 / beansNum;
    int first, second;
    float firstValue, secondValue;

    float sigma = surSize*0.5;
    float dx, dy;
    float radA = p.angle * M_PI / 180;

    int curGistX, curGistY;
    float topLeftCenterX, topLeftCenterY;

    for(int i = -surSize/2 - sqrt(2); i < surSize/2 + sqrt(2); i++)
        for(int j = -surSize/2 - sqrt(2); j < surSize/2 + sqrt(2); j++)
        {
            weight = magnitudes->getPixel(p.y+i, p.x+j)*(pow(M_E,-(i*i + j*j)/(2*sigma*sigma)))/(2*M_PI*sigma*sigma);
            angle = - p.angle + angles->getPixel(p.y+i, p.x+j);
            while(angle < 0) angle += 360;

            dx = j * cos(radA) + i*sin(radA);
            dy = - j * sin(radA) + i*cos(radA);

            curGistX = (int)(dy + surSize/2) / gistSize;
            curGistY = (int)(dx + surSize/2) / gistSize;

            if(dx < curGistX*gistSize + gistSize/2) curGistX = (curGistX - 1 + gistNum)%gistNum;
            if(dy > curGistY*gistSize + gistSize/2) curGistY = (curGistY - 1 + gistNum)%gistNum;

            topLeftCenterX = curGistX*gistSize + gistSize/2;
            topLeftCenterY = curGistY*gistSize + gistSize/2;

            //add in top left
            curGistNum = (curGistY)*gistNum + curGistX;
            descriptor->addInGist(curGistNum, angle, weight*(gistSize - dy + topLeftCenterY)*(gistSize - dx + topLeftCenterX) / pow(gistSize, 2), beansNum);

            //add in top right
            curGistNum = (curGistY)*gistNum + (curGistX + 1)%gistNum;
            descriptor->addInGist(curGistNum, angle, weight*(gistSize - dy + topLeftCenterY)*(dx - topLeftCenterX) / pow(gistSize, 2), beansNum);

            //add in bottom left
            curGistNum = ((curGistY + 1) % gistNum)*gistNum + curGistX;
            descriptor->addInGist(curGistNum, angle, weight*(dy - topLeftCenterY)*(gistSize - dx + topLeftCenterX) / pow(gistSize, 2), beansNum);

            //add in bottom right
            curGistNum = ((curGistY + 1) % gistNum)*gistNum + (curGistX + 1)%gistNum;
            descriptor->addInGist(curGistNum, angle, weight*(dy - topLeftCenterY)*(dx - topLeftCenterX) / pow(gistSize, 2), beansNum);
        }
    descriptor->normalize();
    return descriptor;
}

DescriptorGenerator::~DescriptorGenerator()
{
}

