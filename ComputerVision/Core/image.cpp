#include "image.h"


Image::Image(int h, int w)
{
    image = make_unique<float[]>(h*w);
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
    float color;

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

shared_ptr<Image> Image::getNoisy(const Image& img)
{
    shared_ptr<Image> noisy = make_shared<Image>(img.getHeight(), img.getWidth());
    mt19937 mt(1856);
    uniform_int_distribution<int> dist(0,9);
    uniform_int_distribution<int> dist1(0,255);
    int random;
    for(int i=0; i<img.getHeight(); i++)
    {
        for(int j=0; j<img.getWidth(); j++)
        {
            random = dist(mt);
            if(random > 7)
            {
                noisy->setPixel(i,j,dist1(mt));
            }
            else
            {
                noisy->setPixel(i,j,img.getPixel(i,j));
            }
        }
    }
    return noisy;
}

shared_ptr<Image> Image::changeBrightness(const Image& img, int dBright)
{
    shared_ptr<Image> bright = make_shared<Image>(img.getHeight(), img.getWidth());
    float newPixel;

    for(int i=0; i<img.getHeight(); i++)
    {
        for(int j=0; j<img.getWidth(); j++)
        {
            newPixel = img.getPixel(i,j) + dBright;
            newPixel = min((float)255, max((float)0, newPixel));
            bright->setPixel(i,j,newPixel);
        }
    }

    return bright;
}

shared_ptr<Image> Image::changeContrast(const Image& img, float k)
{
    shared_ptr<Image> contrast = make_shared<Image>(img.getHeight(), img.getWidth());
    float mid = 0;
    float newPixel;
    for(int i=0; i<img.getHeight(); i++)
    {
        for(int j=0; j<img.getWidth(); j++)
        {
            mid += img.getPixel(i,j);
        }
    }
    mid /= img.getHeight() * img.getWidth();

    for(int i=0; i<img.getHeight(); i++)
    {
        for(int j=0; j<img.getWidth(); j++)
        {
            newPixel = mid + k*(img.getPixel(i,j) - mid);
            newPixel = min((float)255, max((float)0, newPixel));
            contrast->setPixel(i,j,newPixel);
        }
    }
    return contrast;
}

QImage Image::toQImage() const
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

bool Image::toFile(const QString &fileName) const
{
    QImage image = toQImage();
    return image.save(fileName, "JPEG");
}

shared_ptr<Image> Image::compress(int scale) const
{
    shared_ptr<Image> result = make_shared<Image>(height/scale, width/scale);

    for(int i=0; i<result->getHeight(); i++)
    {
        for(int j=0; j<result->getWidth(); j++)
        {
            result->setPixel(i,j,getPixel(i*scale, j*scale));
        }
    }
    return result;
}

float Image::getPixel(int i, int j, EdgeMode mode) const
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
                if(i<0) i = i*(-1) - 1;
                if(j<0) j = j*(-1) - 1;
                if(i>=height) i = height - (i%height) - 1;
                if(j>=width) j = width - (j%width) - 1;
                return image[i*width + j];
        }
    }
    return 0;
}

float Image::setPixel(int i, int j, float value)
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

 void Image::normalize(float min, float max, float bottom, float top)
 {
     if(bottom == numeric_limits<float>::max()  || top ==  numeric_limits<float>::min() )
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

int Image::getHeight() const
{
    return height;
}

int Image::getWidth() const
{
    return width;
}
