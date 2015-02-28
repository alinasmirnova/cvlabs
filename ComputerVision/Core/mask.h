#ifndef MASK_H
#define MASK_H
#include <memory>
using namespace std;

class Mask
{
    unique_ptr<int[]> core;
public:
    Mask(int size);
    ~Mask();
};

#endif // MASK_H
