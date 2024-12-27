#include "Level.h"

Level::Level(Point2D* _size, char* _map)
{
    this->size = _size;
    this->map = _map;
}
Point2D* Level::getMapSize()
{
    return this->size;
}
char* Level::getMap()
{
    return this->map;
}