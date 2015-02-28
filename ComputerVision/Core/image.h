#ifndef IMAGE_H
#define IMAGE_H
#include <memory>
using namespace std;

class Image
{
public:
    Image(int h, int w);

private:
    unique_ptr<int[]> image;
    int height;
    int weight;
};

#endif // IMAGE_H
