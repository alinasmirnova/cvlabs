#ifndef MODELPARAMETER_H
#define MODELPARAMETER_H


struct ModelParameter
{
    int x;
    int y;
    int scale;
    int angle;
public:
    ModelParameter(int x, int y, int scale, int angle) : x(x), y(y), scale(scale), angle(angle){}
    ~ModelParameter(){}

    bool operator< (const ModelParameter &p) const {
           if (scale != p.scale)
               return scale < p.scale;
           if (angle != p.angle)
               return angle < p.angle;
           if (x != p.x)
               return x < p.x;
           return y < p.y;
       }
};

#endif
