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
    unique_ptr<Image> gradients;
    unique_ptr<Image> angles;
public:
    DescriptorGenerator(const Image& image);

    Descriptor getDescriptor(int x, int y, int surSize, int gistNum, int basketNum);

    ~DescriptorGenerator();
};

#endif
