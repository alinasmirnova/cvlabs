#include "image.h"

Image::Image(int w, int h)
{
    width = w;
    height = h;
    image = new short*[height];
    for(int i=0; i<height; i++)
    {
        image[i] = new short[width];
    }
}

Image* Image::GetFromFile(const char *fileName)
{
    QImage* qImage = new QImage();
    bool isLoaded = qImage->load(fileName);
    if(isLoaded)
    {
        qDebug()<< "Loaded QImage from file";
        return GetFromQImage(qImage);
    }
    else
    {
        qDebug()<< "Can\'t load QImage from file "<<fileName;
        return NULL;
    }
}

Image* Image::GetFromQImage(QImage *qImage)
{
    if(qImage == NULL)
    {
        qDebug()<<"Can\'t create Image from NULL";
        return NULL;
    }

    Image *result = new Image(qImage->width(), qImage->height());
    int pixelColor;
    for(int i=0; i<result->height; i++)
    {
        for(int j=0; j<result->width; j++)
        {
            pixelColor = qImage->pixel(j, i);
            result->image[i][j] = (short)(0.299 * qRed(pixelColor)
                                        + 0.587 * qGreen(pixelColor)
                                        + 0.114 * qBlue(pixelColor));
        }
    }
    qDebug()<< "Image of size "<< result->height<<"x"<<result->width<<" was created";
    return result;
}

QImage* Image::toQImage()
{
    QImage *result = new QImage(width, height, QImage::Format_RGB32);
    QRgb color;

    for(int i=0; i<height; i++)
    {
       for(int j=0; j<width; j++)
       {
           color = qRgb(image[i][j], image[i][j], image[i][j]);
           result->setPixel(j, i, color);
       }
    }
    return result;
}

