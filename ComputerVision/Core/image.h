#ifndef IMAGE_H
#define IMAGE_H
#include <QImage>
#include <QColor>
#include <QDebug>

class Image
{
    short **image;
    int width;
    int height;
    Image(int w, int h);
public:

    static Image* GetFromFile(const char* fileName);
    static Image* GetFromQImage(QImage* qImage);

    QImage* toQImage();
};

#endif // IMAGE_H
