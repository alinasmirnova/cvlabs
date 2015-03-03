#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H
#include"image.h"
#include"mask.h"
#include"maskfactory.h"
#include"separatedmask.h"
class FilterManager
{
public:
    static shared_ptr<Image> Filter(shared_ptr<Image> source, shared_ptr<Mask> mask, bool norm = true);
    static shared_ptr<Image> SeparatedFilter(shared_ptr<Image> source, shared_ptr<SeparatedMask> mask, bool norm = true);

    static shared_ptr<Image> SobelOperator(shared_ptr<Image> source);
    static shared_ptr<Image> SobelOperatorSeparated(shared_ptr<Image> source);

};

#endif // FILTERMANAGER_H
