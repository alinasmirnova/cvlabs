#include "separatedmask.h"

shared_ptr<Mask> SeparatedMask::setRow(shared_ptr<Mask> r)
{
    if(r->getHeight() == 1)
    {
        row = r;
        return row;
    }
    else
    {
        qFatal("setRow: wrong row height");
        return nullptr;
    }
}

shared_ptr<Mask> SeparatedMask::setColumn(shared_ptr<Mask> c)
{
    if(c->getWidth() == 1)
    {
        column = c;
        return column;
    }
    else
    {
        qFatal("setColum: wrong column width");
        return nullptr;
    }
}

shared_ptr<Mask> SeparatedMask::getRow() const
{
    return row;
}

shared_ptr<Mask> SeparatedMask::getColumn() const
{
    return column;
}

SeparatedMask::~SeparatedMask()
{

}

