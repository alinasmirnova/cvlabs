#ifndef IMAGE_H
#define IMAGE_H
#include <memory>
#include <QImage>
#include <QtDebug>
using namespace std;

class Image
{
public:
    Image(int h, int w);

    static shared_ptr<Image> fromFile(const QString &fileName);
    static shared_ptr<Image> fromQImage(QImage picture);

    QImage toQImage();
    void normalize(int min, int max, int bottom = INT_MAX, int top = INT_MIN);

    int getPixel(int i, int j);
    int setPixel(int i, int j, int value);

    int getHeight();
    int getWidth();

private:
    unique_ptr<int[]> image;
    int height;
    int width;
};

#endif // IMAGE_H
