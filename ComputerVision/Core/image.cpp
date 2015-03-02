#include "image.h"


Image::Image(int h, int w)
{
    image = unique_ptr<int[]>(new int[h*w]);
    height = h;
    width = w;
}

shared_ptr<Image> Image::fromFile(const QString& fileName)
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
    shared_ptr<Image> result = make_shared<Image>(picture.height(), picture.width());
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

QImage Image::toQImage()
{
    QImage result = QImage(width, height, QImage::Format_RGB32);
    int color;
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            color = getPixel(i, j);
            result.setPixel(j, i, qRgb(color,color,color));
        }
    }
    return result;
}

int Image::getPixel(int i, int j)
{
    if(i<height && j<width && i>=0 && j>=0)
    {
        return image[i*width + j];
    }
    else
    {
        qFatal("getPixel: index out of the range");
        return -1;
    }
}

int Image::setPixel(int i, int j, int value)
{
    if(i<height && j<width && i>=0 && j>=0)
    {
        image[i*width + j] = value;
        return value;
    }
    else
    {
        qFatal("setPixel: index out of the range");
        return -1;
    }
}

 void Image::normalize(int min, int max, int bottom, int top)
 {
     if(bottom == INT_MAX || top == INT_MIN)
     {
         for(int i=0; i<height; i++)
         {
             for(int j=0; j<width; j++)
             {
                 if(image[i*width+j] < bottom)
                 {
                     bottom = image[i*width+j];
                 }
                 else if(image[i*width+j] > top)
                 {
                     top = image[i*width+j];
                 }
             }
         }
     }
     for(int i=0; i<height; i++)
     {
         for(int j=0; j<width; j++)
         {
            image[i*width+j] = min + (max - min)*(image[i*width+j] - bottom)/(top - bottom);
         }
     }
 }

int Image::getHeight()
{
    return height;
}

int Image::getWidth()
{
    return width;
}
