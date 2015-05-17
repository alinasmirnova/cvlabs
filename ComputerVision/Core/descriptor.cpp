#include "descriptor.h"

Descriptor::Descriptor(int binsNum, Point p): binsNum(binsNum), point(p)
{
    bins = make_unique<float[]>(binsNum);
}

float Descriptor::distance(const Descriptor& d1, const Descriptor& d2)
{
    float distance = 0;

    for(int i=0; i<d1.binsNum; i++)
    {
        distance += pow(d1.bins[i] - d2.bins[i], 2);
    }

    return sqrt(distance);
}

void Descriptor::simpleNormalize()
{
    float lenght = 0;

    for(int i=0; i<binsNum; i++)
    {
        lenght += pow(bins[i], 2);
    }

    for(int i=0; i<binsNum; i++)
    {
        bins[i] = bins[i] / sqrt(lenght);
    }
}

void Descriptor::normalize()
{
    simpleNormalize();
    for(int i = 0; i<binsNum; i++)
    {
        bins[i] = min(bins[i], 0.2f);
    }
    simpleNormalize();
}

void Descriptor::addInBin(int binNum, float value)
{
    if(binNum >= 0 && binNum < binsNum)
    {
        bins[binNum] += value;
    }
}

void Descriptor::addInHist(int curHistNum, float angle, float weight, int binInHist)
{
    float oneBean = 360.0 / binInHist;
    int first = angle/oneBean;
    int second;

    if(angle < first*oneBean + 0.5*oneBean)
    {
        second = first - 1;
        if(second < 0) second += binInHist;
    }
    else
    {
        second = (first + 1)%binInHist;
    }

    float r = fabs(first*oneBean + 0.5*oneBean - angle);
    float firstValue = weight*(oneBean - r)/oneBean;
    float secondValue = weight - firstValue;

    addInBin(curHistNum*binInHist + first, firstValue);
    addInBin(curHistNum*binInHist + second, secondValue);
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

float Descriptor::getAngle(int bin)
{
    float oneBean = 360.0/binsNum;

    int left = (bin - 1 + binsNum) % binsNum;
    int right = (bin + 1) % binsNum;

    float y2 = bins[bin];
    float x2 = bin * oneBean + oneBean/2;

    float y1 = bins[left];
    float x1 = x2 - oneBean;

    float y3 = bins[right];
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

    for(int i=0; i<binsNum; i++)
    {
        if(bins[i] > maxValue1)
        {
            maxValue2 = maxValue1;
            maxIndex2 = maxIndex1;

            maxValue1 = bins[i];
            maxIndex1 = i;
        }
        else if(bins[i] > maxValue2)
        {
            maxValue2 = bins[i];
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

