#include "descriptorgenerator.h"

DescriptorGenerator::DescriptorGenerator(const Image& image)
{
    auto dx = FilterManager::SeparatedFilter(image, *MaskFactory::SobelSeparated(Asix::X), false, EdgeMode::MIRROR);
    auto dy = FilterManager::SeparatedFilter(image, *MaskFactory::SobelSeparated(Asix::Y), false, EdgeMode::MIRROR);

    gradients = make_unique<Image>(image.getHeight(), image.getWidth());
    angles = make_unique<Image>(image.getHeight(), image.getWidth());

    for(int i=0; i<image.getHeight(); i++)
    {
        for(int j=0; j<image.getWidth(); j++)
        {
            angles->setPixel(i,j,atan2(dy->getPixel(i,j), dx->getPixel(i, j))*180/M_PI + 180);

            gradients->setPixel(i, j, sqrt(pow(dx->getPixel(i,j),2) + pow(dx->getPixel(i,j),2)));
        }
    }
}

shared_ptr<Descriptor> DescriptorGenerator::getDescriptor(int x, int y, int surSize, int gistNum, int basketNum)
{
    auto descriptor = make_shared<Descriptor>(basketNum*gistNum, x, y);

    int gistSize = ceil(surSize/gistNum);
    int curGistNum;
    float weight,angle;
    float oneBasket = 360 / basketNum;
    int left;
    float leftValue;

    x -= surSize/2;
    y -= surSize/2;
    float sigma = surSize*0.5;
    int x1,y1;

    for(int i=0; i<gistNum; i++)
    {
        for(int j=0; j<gistNum; j++)
        {
            curGistNum = i*gistNum+j;
            for(int curX = i*gistSize; curX<(i+1)*gistSize && curX<surSize; curX++)
            {
                for(int curY = j*gistSize; curY<(j+1)*gistSize && curY<surSize; curY++)
                {
                    x1 = curX -surSize/2;
                    y1 = curY - surSize/2;

                    weight = gradients->getPixel(y+curY, x+curX)*(pow(M_E,-(x1*x1 + y1*y1)/(2*sigma*sigma)))/(2*M_PI*sigma*sigma);//mask->getPixel(curY, curX)*gradients->getPixel(y+curY, x+curX);
                    angle = angles->getPixel(y+curY, x+curX);
                    left = angle/oneBasket;

                    leftValue = weight*(angle - left*oneBasket)/(oneBasket);

                    descriptor->addInBasket(curGistNum*basketNum + left, leftValue);
                    descriptor->addInBasket(curGistNum*basketNum + left + 1, weight - leftValue);
                }
            }
        }
    }

    descriptor->normalize();
    return descriptor;
}

DescriptorGenerator::~DescriptorGenerator()
{
}

