#include "detectors.h"

Detectors::Detectors()
{

}

float Detectors::findContrast(const Image &image, int x, int y, int dx, int dy, int halfSizeW)
{
    if(dx == dy && dx == 0) return std::numeric_limits<float>::max();
    float sum = 0;
    float p1, p2;
    for(int i = y - halfSizeW; i<y+halfSizeW; i++)
    {
        for(int j = x - halfSizeW; j<x+halfSizeW; j++)
        {
            p1 = image.getPixel(i, j, EdgeMode::MIRROR);
            p2 = image.getPixel(i + dy, j + dx, EdgeMode::MIRROR);
            sum = sum + pow(p1 - p2, 2);
        }
    }
    return sum;
}

vector<Point> Detectors::Moravec(const Image &image, int halfSizeW, int localMaxSize, float bottom)
{
    Image S(image.getHeight(),image.getWidth());
    float C;
    float min;
    for(int i=0; i<image.getHeight(); i++)
    {
        for(int j=0; j<image.getWidth(); j++)
        {
            min = std::numeric_limits<float>::max();
            for(int dx = -1; dx<=1; dx++)
            {
                for(int dy=-1; dy<=1; dy++)
                {
                    C = findContrast(image, j, i, dx,dy, halfSizeW);
                    if(C < min)
                    {
                         min = C;
                    }
                }
            }
            S.setPixel(i,j,min);
        }
    }
    S.normalize(0,1);
    return findLocalMax(S, image.getHeight(), image.getWidth(), localMaxSize, bottom);
}

vector<Point> Detectors::findLocalMax(const Image &S, int h, int w, int cellSize, float bottom)
{
     vector<Point> points;
     int cellNumY = (h%cellSize ==0) ? h/cellSize : h/cellSize + 1;
     int cellNumX = (w%cellSize ==0) ? w/cellSize : w/cellSize + 1;

     float max;
     int maxX, maxY;

     for(int i = 0; i<cellNumY; i++)
     {
         for(int j=0; j<cellNumX; j++)
         {
             max = numeric_limits<float>::min();
             for(int y = i*cellSize; y < (i+1)*cellSize && y < h; y++)
             {
                 for(int x = j*cellSize; x < (j+1)*cellSize && x < w; x++)
                 {
                     if(S.getPixel(y, x) > max)
                     {
                         max = S.getPixel(y, x);
                         maxX = x;
                         maxY = y;
                     }
                 }
             }
             if(max > bottom)
             {
                 points.push_back(Point(maxX, maxY));
             }
         }
     }
     return points;
}


Detectors::~Detectors()
{

}

