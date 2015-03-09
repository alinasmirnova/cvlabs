#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H
#include"image.h"
#include"mask.h"
#include"maskfactory.h"
#include"separatedmask.h"
class FilterManager
{
public:
    static shared_ptr<Image> Filter(const Image &source, const Mask& mask, bool norm = true, EdgeMode edgeMode = EdgeMode::ZEROS);
    static shared_ptr<Image> SeparatedFilter(const Image& source, const SeparatedMask &mask, bool norm = true, EdgeMode edgeMode = EdgeMode::ZEROS);

    static shared_ptr<Image> SobelOperator(const Image &source, EdgeMode edgeMode = EdgeMode::ZEROS);
    static shared_ptr<Image> SobelOperatorSeparated(const Image &source, EdgeMode edgeMode = EdgeMode::ZEROS);

};

#endif
