#ifndef IMAGE_H
#define IMAGE_H
#include <memory>
#include <QImage>
#include <QDebug>
using namespace std;

class Image
{
    Image(int h, int w);

public:
    static shared_ptr<Image> fromFile(const char* fileName);
    static shared_ptr<Image> fromQImage(QImage picture);

    QImage toQImage();

    int getPixel(int i, int j);
    int setPixel(int i, int j, int value);

private:
    unique_ptr<int[]> image;
    int height;
    int weight;
};

#endif // IMAGE_H
