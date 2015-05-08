#ifndef POINT_H
#define POINT_H

struct Point {
    int x;
    int y;
    float contrast;
    float scale;
    float angle;

    Point(int x, int y, float c, float s): x(x), y(y), contrast(c), scale(s){}
    Point(){}
};

#endif // POINT_H

