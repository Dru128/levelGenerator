#pragma once
namespace LevelGenerator
{
    class Point2D {
    protected:
        int x, y;
    public:
        Point2D(int _x, int _y);
        Point2D(const Point2D* point);
        ~Point2D();
        int getX();
        void setX(int _x);
        int getY();
        void setY(int _y);
    };
}