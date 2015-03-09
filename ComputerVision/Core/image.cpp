#include "image.h"


Image::Image(int h, int w)
{
    image = make_unique<double[]>(h*w);
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
    double color;

    for(int i=0; i<picture.height(); i++)
    {
        for(int j=0; j<picture.width(); j++)
        {
            original = picture.pixel(j,i);
            color = 0.299 * qRed(original) +
                    0.587 * qGreen(original) +
                    0.114 * qBlue(original);
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
            color = qRound(getPixel(i, j));
            result.setPixel(j, i, qRgb(color,color,color));
        }
    }
    return result;
}

double Image::getPixel(int i, int j, EdgeMode mode)
{
    if(i<height && j<width && i>=0 && j>=0)
    {
        return image[i*width + j];
    }
    else
    {
        switch(mode)
        {
            case EdgeMode::ZEROS: return 0;
            case EdgeMode::COPY:
                 return image[min(max(i,0), height-1)*width + min(max(j,0), width-1)];
            case EdgeMode::MIRROR:
                if(i<0) i = i*(-1);
                if(j<0) j = j*(-1);
                if(i>height) i = height - (i%height);
                if(j>width) j = width - (j%width);
                return image[i*width + j];
        }
    }
    return 0;
}

int Image::setPixel(int i, int j, double value)
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

 void Image::normalize(double min, double max, double bottom, double top)
 {
     if(bottom == numeric_limits<double>::max()  || top ==  numeric_limits<double>::min() )
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
