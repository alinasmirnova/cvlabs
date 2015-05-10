#ifndef DESCRIPTORGENERATOR_H
#define DESCRIPTORGENERATOR_H
#include "image.h"
#include "filtermanager.h"
#include "maskfactory.h"
#include "descriptor.h"
#include <vector>

using namespace std;

class DescriptorGenerator
{
    unique_ptr<Image> magnitudes;
    unique_ptr<Image> angles;
public:
    DescriptorGenerator(const Image& image);

    shared_ptr<Descriptor> getDescriptor(Point p, int surSize, int gistNum, int beansNum);
    shared_ptr<Descriptor> getAngleDescriptor(Point p, int surSize);

    ~DescriptorGenerator();
};

#endif
