#ifndef IMAGE_H
#define IMAGE_H
#include <memory>
#include <limits>
#include <QImage>
#include <QtDebug>
#include <math.h>
using namespace std;

enum EdgeMode
{
    ZEROS = 1,
    MIRROR = 2,
    COPY = 3
};

class Image
{
public:
    Image(int h, int w);

    static shared_ptr<Image> fromFile(const QString &fileName);
    static shared_ptr<Image> fromQImage(QImage picture);

    QImage toQImage() const;
    bool toFile(const QString &fileName)const;

    shared_ptr<Image> compress(int scale) const;
    void normalize(float min, float max, float bottom = numeric_limits<float>::max(), float top = numeric_limits<float>::min());

    float getPixel(int i, int j, EdgeMode mode=EdgeMode::ZEROS) const;
    float setPixel(int i, int j, float value);

    int getHeight() const;
    int getWidth() const;

private:
    unique_ptr<float[]> image;
    int height;
    int width;
};

#endif // IMAGE_H
