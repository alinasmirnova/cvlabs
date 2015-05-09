#ifndef POINT_H
#define POINT_H

struct Point {
    int x;
    int y;
    float contrast;
    float scale;
    float innerScale;
    float angle;

    Point(int x, int y, float c, float s): x(x), y(y), contrast(c), scale(s)
    {
        innerScale = 1;
    }

    Point(int x, int y, float c, float s, float inner): x(x), y(y), contrast(c), scale(s), innerScale(inner)
    {}
    Point(){}
};

#endif // POINT_H

