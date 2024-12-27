#include "Point2D.h"

Point2D::Point2D(int _x, int _y)
{
    x = _x;
    y = _y;
}
Point2D::Point2D(const Point2D* point)
{
    x = point->x;
    y = point->y;
}
Point2D::~Point2D() {}
int Point2D::getX()
{
    return this->x;
}
void Point2D::setX(int _x)
{
    x = _x;
}
int Point2D::getY()
{
    return y;
}
void Point2D::setY(int _y)
{
    y = _y;
}