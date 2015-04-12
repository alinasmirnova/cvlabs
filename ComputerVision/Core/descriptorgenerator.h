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
public:
    DescriptorGenerator(const Image& image, float sigma);

    shared_ptr<Descriptor> getDescriptor(int x, int y, int surSize, int bucketNum);

    ~DescriptorGenerator();
};

#endif // DESCRIPTORGENERATOR_H
