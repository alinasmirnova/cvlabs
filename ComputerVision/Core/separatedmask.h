#ifndef SEPARATEDMASK_H
#define SEPARATEDMASK_H
#include <memory>
#include <QDebug>
using namespace std;
class SeparatedMask
{
public:
    SeparatedMask(int size);
    float getRowPixel(int i);
    float getColPixel(int i);

    float setRowPixel(int i, float value);
    float setColPixel(int i, float value);

    int size();

    ~SeparatedMask();
private:
    int s;
    unique_ptr<float[]> row;
    unique_ptr<float[]> column;
};

#endif // SEPARATEDMASK_H
