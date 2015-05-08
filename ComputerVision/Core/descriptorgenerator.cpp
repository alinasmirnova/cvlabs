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
    auto descriptor = make_shared<Descriptor>(beansNum*gistNum, p);

    int gistSize = ceil(surSize*p.scale/gistNum);
    int curGistNum;
    float weight,angle;
    float oneBasket = 360 / beansNum;
    int left;
    float leftValue;

    int x = p.x - surSize/2;
    int y = p.y - surSize/2;
    float sigma = surSize*0.5;

    for(int i=0; i<surSize; i++)
    {
        for(int j=0; j<surSize; j++)
        {
            curGistNum = (i/gistNum)*gistNum + (j/gistNum);
            weight = magnitudes->getPixel(y+i, x+j)*(pow(M_E,-(i*i + j*j)/(2*sigma*sigma)))/(2*M_PI*sigma*sigma);
            angle = angles->getPixel(y+i, x+j);
            left = angle/oneBasket;

            leftValue = weight*(angle - left*oneBasket + oneBasket/2)/(oneBasket);

            descriptor->addInBean(curGistNum*beansNum + left, leftValue);
            descriptor->addInBean(curGistNum*beansNum + (left + 1)%beansNum, weight - leftValue);
        }
    }

    descriptor->normalize();
    return descriptor;
}

DescriptorGenerator::~DescriptorGenerator()
{
}

