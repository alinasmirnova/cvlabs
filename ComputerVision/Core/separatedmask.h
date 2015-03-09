#ifndef SEPARATEDMASK_H
#define SEPARATEDMASK_H
#include <memory>
#include <QDebug>
#include "mask.h"
using namespace std;
class SeparatedMask
{
public:
    shared_ptr<Mask> setRow(shared_ptr<Mask> r);
    shared_ptr<Mask> setColumn(shared_ptr<Mask> c);

    shared_ptr<Mask> getRow() const;
    shared_ptr<Mask> getColumn() const;

    ~SeparatedMask();
private:
    shared_ptr<Mask> row;
    shared_ptr<Mask> column;
};

#endif
