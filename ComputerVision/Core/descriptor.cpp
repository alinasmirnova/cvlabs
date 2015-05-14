#include "descriptor.h"

Descriptor::Descriptor(int beansNum, Point p): beansNum(beansNum), point(p)
{
    beans = make_unique<float[]>(beansNum);
}

float Descriptor::distance(const Descriptor& d1, const Descriptor& d2)
{
    float distance = 0;

    for(int i=0; i<d1.beansNum; i++)
    {
        distance += pow(d1.beans[i] - d2.beans[i], 2);
    }

    return sqrt(distance);
}

void Descriptor::simpleNormalize()
{
    float lenght = 0;

    for(int i=0; i<beansNum; i++)
    {
        lenght += pow(beans[i], 2);
    }

    for(int i=0; i<beansNum; i++)
    {
        beans[i] = beans[i] / sqrt(lenght);
    }
}

void Descriptor::normalize()
{
    simpleNormalize();
    for(int i = 0; i<beansNum; i++)
    {
        beans[i] = min(beans[i], 0.2f);
    }
    simpleNormalize();
}

void Descriptor::addInBean(int beanNum, float value)
{
    if(beanNum >= 0 && beanNum < beansNum)
    {
        beans[beanNum] += value;
    }
}

void Descriptor::addInGist(int curGistNum, float angle, float weight, int beanInGist)
{
    float oneBean = 360.0 / beanInGist;
    int first = angle/oneBean;
    int second;

    if(angle < first*oneBean + 0.5*oneBean)
    {
        second = first - 1;
        if(second < 0) second += beanInGist;
    }
    else
    {
        second = (first + 1)%beanInGist;
    }

    float r = fabs(first*oneBean + 0.5*oneBean - angle);
    float firstValue = weight*(oneBean - r)/oneBean;
    float secondValue = weight - firstValue;

    addInBean(curGistNum*beanInGist + first, firstValue);
    addInBean(curGistNum*beanInGist + second, secondValue);
}

int Descriptor::findClosest(vector<shared_ptr<Descriptor>> descriptors)
{
    int minIndex1 = -1;
    float minValue1 = numeric_limits<float>::max(), minValue2 = numeric_limits<float>::max(), value;

    for(int i=0; i<(int)descriptors.size(); i++)
    {
        value = distance(*this, *descriptors[i]);
        if(value < minValue1)
        {
            minValue2 = minValue1;

            minValue1 = value;
            minIndex1 = i;
        }
        else if(value < minValue2)
        {
            minValue2 = value;
        }
    }
    if(minValue1 / minValue2 < 0.8)
    {
        return minIndex1;
    }
    return -1;
}

float Descriptor::getAngle(int bean)
{
    float oneBean = 360.0/beansNum;

    int left = (bean - 1 + beansNum) % beansNum;
    int right = (bean + 1) % beansNum;

    float y2 = beans[bean];
    float x2 = bean * oneBean + oneBean/2;

    float y1 = beans[left];
    float x1 = x2 - oneBean;

    float y3 = beans[right];
    float x3 = x2 + oneBean;

    float a = (y3 - (x3*(y2-y1)+x2*y1-x1*y2)/(x2-x1)) / (x3*(x3-x1-x2) + x1*x2);
    float b = -(a*(x1+x2)) +((y2 - y1)/(x2 - x1));

    if(a == 0) return 0;
    return -b/(2*a);
}

pair<float,float> Descriptor::getMaxAngle()
{
    int maxIndex1 = -1, maxIndex2 = -1;
    float maxValue1 = numeric_limits<float>::min(), maxValue2 = numeric_limits<float>::min();

    for(int i=0; i<beansNum; i++)
    {
        if(beans[i] > maxValue1)
        {
            maxValue2 = maxValue1;
            maxIndex2 = maxIndex1;

            maxValue1 = beans[i];
            maxIndex1 = i;
        }
        else if(beans[i] > maxValue2)
        {
            maxValue2 = beans[i];
            maxIndex2 = i;
        }
    }


    if(maxValue2 / maxValue1 > 0.8)
    {
        return make_pair<float, float>(getAngle(maxIndex1), getAngle(maxIndex2));
    }
    return make_pair<float, float>(getAngle(maxIndex1), numeric_limits<float>::min());
}

Descriptor::~Descriptor()
{

}

