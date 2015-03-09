#ifndef IMAGE_H
#define IMAGE_H
#include <memory>
#include <limits>
#include <QImage>
#include <QtDebug>
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

    QImage toQImage();
    void normalize(double min, double max, double bottom = INT_MAX, double top = INT_MIN);

    double getPixel(int i, int j, EdgeMode mode=EdgeMode::ZEROS);
    double setPixel(int i, int j, double value);

    int getHeight();
    int getWidth();

private:
    unique_ptr<double[]> image;
    int height;
    int width;
};

#endif // IMAGE_H
