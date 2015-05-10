#ifndef POINT_H
#define POINT_H

class Point {

public:
    int x;
    int y;
    float contrast;
    float scale;
    float innerScale;
    float angle;

    Point(int x, int y, float c, float s): x(x), y(y), contrast(c), scale(s)
    {
        innerScale = 1;
        angle = 0;
    }

    Point(int x, int y, float c, float s, float inner): x(x), y(y), contrast(c), scale(s), innerScale(inner)
    {
        angle = 0;
    }

    Point()
    {

    }
};

#endif // POINT_H

