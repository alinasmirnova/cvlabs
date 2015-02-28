#include "image.h"


Image::Image(int h, int w)
{
    image = unique_ptr<int[]>(new int[h*w]);
    height = h;
    weight = w;
}

shared_ptr<Image> Image::fromFile(const char* fileName)
{
    QImage picture;
    bool isLoaded = picture.load(fileName);
    if(isLoaded)
    {
        qDebug() << "Load image from file: " << fileName;
        return fromQImage(picture);
    }
    else
    {
        qWarning() << "Failed to load image from file";
        return nullptr;
    }
}

shared_ptr<Image> Image::fromQImage(QImage picture)
{
    shared_ptr<Image> result = make_shared<Image>(picture.height(), picture.width()());
    QRgb original;
    int color;

    for(int i=0; i<picture.height(); i++)
    {
        for(int j=0; j<picture.width(); j++)
        {
            original = picture.pixel(j,i);
            color = qRound(0.299 * qRed(original) +
                           0.587 * qGreen(original) +
                           0.114 * qBlue(original));
            result->setPixel(i,j,color);
        }
    }

    return result;
}

QImage toQImage()
{
    QImage result = QImage(width, height, QImage::Format_RGB32);
    int color;
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<weight; j++)
        {
            color = getPixel(i, j);
            result.setPixel(j, i, qRgb(color,color,color));
        }
    }
    return result;
}

int getPixel(int i, int j)
{
    if(i<height && j<weight && i>0 && j>0)
    {
        return image[i*weight + j];
    }
    else
    {
        qFatal() << "Index out of the range: [" << i << "][" << j << "]";
        return -1;
    }
}

int setPixel(int i, int j, int value)
{
    if(i<height && j<weight && i>0 && j>0)
    {
        if(value < 0)
        {
            value = 0;
        }
        else if(value > 255)
        {
            value = 255;
        }

        image[i*weight + j] = value;
        return value;
    }
    else
    {
        qFatal() << "Index out of the range: [" << i << "][" << j << "]";
        return -1;
    }
}
