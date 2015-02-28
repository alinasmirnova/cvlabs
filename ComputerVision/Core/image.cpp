#include "image.h"


Image::Image(int h, int w)
{
    image = unique_ptr<int[]>(new int[h*w]);
    height = h;
    weight = w;
}
