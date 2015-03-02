#include "maskfactory.h"

MaskFactory::MaskFactory()
{

}

shared_ptr<Mask> MaskFactory::Shift(int pixelNum, Direction direction)
{
    shared_ptr<Mask> mask = make_shared<Mask>(pixelNum*2+1);
    for(int i=0; i<mask->size(); i++)
    {
        for(int j=0; j<mask->size(); j++)
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
            mask->setPixel(pixelNum, mask->size() - 1, 1);
            break;
        case Direction::DOWN:
            mask->setPixel(mask->size() - 1, pixelNum, 1);
            break;
        case Direction::LEFT:
            mask->setPixel(pixelNum, 0, 1);
            break;
    }
    return mask;
}

shared_ptr<Mask> MaskFactory::Blur(int k, int blur)
{
    shared_ptr<Mask> mask = make_shared<Mask>(k*2+1);
    for(int i=0; i<mask->size(); i++)
    {
        for(int j=0; j<mask->size(); j++)
        {
            mask->setPixel(i,j, 1./blur);
        }
    }
    return mask;
}

shared_ptr<Mask> MaskFactory::Sobel(Asix asix)
{
    shared_ptr<Mask> result = make_shared<Mask>(3);
    float *mask;

    switch(asix)
    {
    case Asix::X:
        mask = new float[9]{-1,0,1,-2,0,2,-1,0,1};
        break;
    case Asix::Y:
        mask = new float[9]{-1,-2,-1,0,0,0,1,2,1};
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
    shared_ptr<Mask> result = make_shared<Mask>(3);
    float *mask;

    switch(asix)
    {
    case Asix::X:
        mask = new float[9]{-1,0,1,-1,0,1,-1,0,1};
        break;
    case Asix::Y:
        mask = new float[9]{-1,-1,-1,0,0,0,1,1,1};
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
    shared_ptr<Mask> result = make_shared<Mask>(3);
    float *mask;

    switch(asix)
    {
    case Asix::X:
        mask = new float[9]{-3,0,3,-10,0,10,-3,0,3};
        break;
    case Asix::Y:
        mask = new float[9]{-3,-10,-3,0,0,0,3,10,3};
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

shared_ptr<Mask> MaskFactory::Gauss(int k, int theta)
{
    shared_ptr<Mask> result = make_shared<Mask>(2*k+1);
    int x, y;
    float value;
    for(int i=0; i<result->size(); i++)
    {
        for(int j=0; j<result->size(); j++)
        {
            x = i - k;
            y = j - k;
            value = (pow(M_E,-(x*x + y*y)/(2*theta*theta)))/(2*M_PI*theta*theta);
            result->setPixel(i,j,value);
        }
    }
    return result;
}

shared_ptr<SeparatedMask> MaskFactory::SobelSeparated(Asix asix)
{
    shared_ptr<SeparatedMask> result = make_shared<SeparatedMask>(3);
    float *row;
    float *column;

    switch(asix)
    {
    case Asix::X:
        row = new float[3]{-1,0,1};
        column = new float[3]{1,2,1};
        break;
    case Asix::Y:
            row = new float[3]{1,2,1};
            column = new float[3]{1,0,-1};
    }

    for(int i=0; i<3; i++)
    {
        result->setRowPixel(i,row[i]);
        result->setColPixel(i, column[i]);
    }
    delete row;
    delete column;
    return result;
}

shared_ptr<SeparatedMask> MaskFactory::GaussSeparated(int k, int theta)
{
    shared_ptr<SeparatedMask> result = make_shared<SeparatedMask>(2*k+1);
    int x;
    float value;
    for(int i=0; i<result->size(); i++)
    {
        x = i - k;
        value = (pow(M_E,-(x*x)/(2*theta*theta)))/(sqrt(2*M_PI)*theta);
        result->setRowPixel(i,value);
        result->setColPixel(i,value);
    }
    return result;
}

MaskFactory::~MaskFactory()
{

}
