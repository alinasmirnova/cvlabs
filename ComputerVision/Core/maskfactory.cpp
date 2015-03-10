#include "maskfactory.h"

MaskFactory::MaskFactory()
{

}

shared_ptr<Mask> MaskFactory::Shift(int pixelNum, Direction direction)
{
    int size = pixelNum*2+1;
    shared_ptr<Mask> mask = make_shared<Mask>(size,size);
    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            mask->setPixel(i,j,0);
        }
    }
    switch(direction)
    {
        case Direction::UP:
            mask->setPixel(0, pixelNum, 1);
            break;
        case Direction::RIGHT:
            mask->setPixel(pixelNum, size - 1, 1);
            break;
        case Direction::DOWN:
            mask->setPixel(size - 1, pixelNum, 1);
            break;
        case Direction::LEFT:
            mask->setPixel(pixelNum, 0, 1);
            break;
    }
    return mask;
}

shared_ptr<Mask> MaskFactory::Blur(int k, int blur)
{
    int size = k*2+1;
    shared_ptr<Mask> mask = make_shared<Mask>(size,size);
    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            mask->setPixel(i,j, 1./blur);
        }
    }
    return mask;
}

shared_ptr<Mask> MaskFactory::Sobel(Asix asix)
{
    shared_ptr<Mask> result = make_shared<Mask>(3,3);
    double *mask;

    switch(asix)
    {
    case Asix::X:
        mask = new double[9]{-1,0,1,-2,0,2,-1,0,1};
        break;
    case Asix::Y:
        mask = new double[9]{-1,-2,-1,0,0,0,1,2,1};
    }

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            result->setPixel(i,j,mask[i*3+j]);
        }
    }
    delete mask;
    return result;
}

shared_ptr<Mask> MaskFactory::Pruit(Asix asix)
{
    shared_ptr<Mask> result = make_shared<Mask>(3,3);
    double *mask;

    switch(asix)
    {
    case Asix::X:
        mask = new double[9]{-1,0,1,-1,0,1,-1,0,1};
        break;
    case Asix::Y:
        mask = new double[9]{-1,-1,-1,0,0,0,1,1,1};
    }

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            result->setPixel(i,j,mask[i*3+j]);
        }
    }
    delete mask;
    return result;
}

shared_ptr<Mask> MaskFactory::Shar(Asix asix)
{
    shared_ptr<Mask> result = make_shared<Mask>(3,3);
    double *mask;

    switch(asix)
    {
    case Asix::X:
        mask = new double[9]{-3,0,3,-10,0,10,-3,0,3};
        break;
    case Asix::Y:
        mask = new double[9]{-3,-10,-3,0,0,0,3,10,3};
    }

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            result->setPixel(i,j,mask[i*3+j]);
        }
    }
    delete mask;
    return result;
}

shared_ptr<Mask> MaskFactory::Gauss(double sigma)
{
    int k = 3*sigma;
    int size = 2*k+1;
    shared_ptr<Mask> result = make_shared<Mask>(size,size);
    int x, y;
    double value;
    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            x = i - k;
            y = j - k;
            value = (pow(M_E,-(x*x + y*y)/(2*sigma*sigma)))/(2*M_PI*sigma*sigma);
            result->setPixel(i,j,value);
        }
    }
    return result;
}

shared_ptr<SeparatedMask> MaskFactory::SobelSeparated(Asix asix)
{
    shared_ptr<SeparatedMask> result = make_shared<SeparatedMask>();
    double *row;
    double *column;

    shared_ptr<Mask> rowMask = make_shared<Mask>(1,3);
    shared_ptr<Mask> columnMask = make_shared<Mask>(3,1);

    switch(asix)
    {
    case Asix::X:
        row = new double[3]{-1,0,1};
        column = new double[3]{1,2,1};
        break;
    case Asix::Y:
        row = new double[3]{1,2,1};
        column = new double[3]{1,0,-1};
    }

    for(int i=0; i<3; i++)
    {
        rowMask->setPixel(0, i, row[i]);
        columnMask->setPixel(i, 0, column[i]);
    }
    result->setColumn(columnMask);
    result->setRow(rowMask);
    delete row;
    delete column;
    return result;
}

shared_ptr<SeparatedMask> MaskFactory::GaussSeparated(double sigma)
{
    int k = 3*sigma;
    int size = 2*k+1;
    shared_ptr<SeparatedMask> result = make_shared<SeparatedMask>();
    shared_ptr<Mask> rowMask = make_shared<Mask>(1,size);
    shared_ptr<Mask> columnMask = make_shared<Mask>(size,1);

    int x;
    double value;
    for(int i=0; i<size; i++)
    {
        x = i - k;
        value = (pow(M_E,-(x*x)/(2*sigma*sigma)))/(sqrt(2*M_PI)*sigma);
        rowMask->setPixel(0, i, value);
        columnMask->setPixel(i, 0, value);
    }
    result->setColumn(columnMask);
    result->setRow(rowMask);
    return result;
}

MaskFactory::~MaskFactory()
{

}
