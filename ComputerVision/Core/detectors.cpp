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

    return findLocalMax(S, localMaxSize, bottom);
}

vector<Point> Detectors::findLocalMax(const Image &S, int localMaxSize, float bottom)
{
     vector<Point> points;
     bool isLocalMax;
     float pixelValue;

     for(int i=0; i<S.getHeight(); i++)
     {
         for(int j=0; j<S.getWidth(); j++)
         {
             isLocalMax = true;
             pixelValue = S.getPixel(i, j);
             if(pixelValue >= bottom)
             {
                 for(int dx = -localMaxSize; dx <= localMaxSize; dx++)
                 {
                     for(int dy = -localMaxSize; dy <= localMaxSize; dy++)
                     {
                         if(pixelValue < S.getPixel(i + dx, j + dy))
                         {
                             isLocalMax = false;
                             break;
                         }
                     }
                     if(!isLocalMax)
                     {
                         break;
                     }
                 }
                 if(isLocalMax)
                 {
                     points.push_back(Point(j, i, pixelValue));
                 }
             }
         }
     }
     return points;
}


vector<Point> Detectors::Harris(const Image &image, int halfSizeW, int localMaxSize, float bottom)
{
    auto A = FilterManager::SeparatedFilter(image, *MaskFactory::SobelSeparated(Asix::X), false, EdgeMode::MIRROR);
    auto C = FilterManager::SeparatedFilter(image, *MaskFactory::SobelSeparated(Asix::Y), false, EdgeMode::MIRROR);
    auto B = make_shared<Image>(image.getHeight(),image.getWidth());

    float x,y;
    for(int i=0; i<image.getHeight(); i++)
    {
        for(int j=0; j<image.getWidth(); j++)
        {
            x = A->getPixel(i,j);
            y = C->getPixel(i,j);
            A->setPixel(i,j,x*x);
            B->setPixel(i,j,x*y);
            C->setPixel(i,j,y*y);
        }
    }
    float sigma = halfSizeW * 1.0 / 3;
    A = FilterManager::SeparatedFilter(*A, *MaskFactory::GaussSeparated(sigma), false, EdgeMode::MIRROR);
    B = FilterManager::SeparatedFilter(*B, *MaskFactory::GaussSeparated(sigma), false, EdgeMode::MIRROR);
    C = FilterManager::SeparatedFilter(*C, *MaskFactory::GaussSeparated(sigma), false, EdgeMode::MIRROR);

    //find min lambda
    auto L = make_shared<Image>(image.getHeight(),image.getWidth());

    float a,b,c;
    float lambda1, lambda2, lambda;
    float d;

    for(int i=0; i<image.getHeight(); i++)
    {
        for(int j=0; j<image.getWidth(); j++)
        {
            a = A->getPixel(i,j);
            b = B->getPixel(i,j);
            c = C->getPixel(i,j);

            d = sqrt(pow(a-c, 2) + 4*pow(b,2));
            lambda1 = (a+c+d)/2;
            lambda2 = (a+c-d)/2;

            lambda = min(lambda1, lambda2) - 0.05*max(lambda1, lambda2);

            L->setPixel(i,j,lambda);
        }
    }

    return findLocalMax(*L, localMaxSize, bottom);
}

float GetDistance(Point p1, Point p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

vector<Point> Detectors::AdaptiveNonMaximumSuppression(const vector<Point> &points, int maxPointNum, int maxR)
{
    vector<Point> sortedPoints = vector<Point>(points);

    if(maxPointNum >= points.size())
    {
        return sortedPoints;
    }
    sort(sortedPoints.begin(), sortedPoints.end(), [](auto a, auto b) {return a.contrast > b.contrast;});

    vector<Point> result;
    vector<bool> isInResult = vector<bool>(sortedPoints.size(), false);

    float r = maxR;
    bool isMax;
    while(result.size() < maxPointNum)
    {
        r = r/2;
        for(uint i=0; i<sortedPoints.size(); i++)
        {
            if(!isInResult[i])
            {
                isMax = true;
                for(uint j=0; j<sortedPoints.size(); j++)
                {
                    if(sortedPoints[i].contrast < sortedPoints[j].contrast && GetDistance(sortedPoints[i], sortedPoints[j]) < r)
                    {
                        isMax = false;
                        break;
                    }
                }

                if(isMax)
                {
                    isInResult[i] = true;
                    result.push_back(sortedPoints[i]);
                    if(result.size() == maxPointNum)
                    {
                        break;
                    }
                }
            }
        }
    }
    return result;
}
