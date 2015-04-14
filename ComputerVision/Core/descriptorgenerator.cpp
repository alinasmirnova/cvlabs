#include "descriptorgenerator.h"

DescriptorGenerator::DescriptorGenerator(const Image& image, float sigma)
{
    shared_ptr<Image> gauss = FilterManager::SeparatedFilter(image, *MaskFactory::GaussSeparated(sigma), false, EdgeMode::MIRROR);
    shared_ptr<Image> dx = FilterManager::SeparatedFilter(*gauss, *MaskFactory::SobelSeparated(Asix::X), false, EdgeMode::MIRROR);
    shared_ptr<Image> dy = FilterManager::SeparatedFilter(*gauss, *MaskFactory::SobelSeparated(Asix::Y), false, EdgeMode::MIRROR);

    gradients = make_unique<Image>(image.getHeight(), image.getWidth());

    for(int i=0; i<image.getHeight(); i++)
    {
        for(int j=0; j<image.getWidth(); j++)
        {
            gradients->setPixel(i,j,atan2(dy->getPixel(i,j), dx->getPixel(i, j)));
        }
    }
}

shared_ptr<Descriptor> DescriptorGenerator::getDescriptor(int x, int y, int surSize, int gistNum, int bucketNum)
{

}

DescriptorGenerator::~DescriptorGenerator()
{

}

