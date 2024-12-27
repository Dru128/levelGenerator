#pragma once
#include "Point2D.h"
// данные уровня
class Level {
private:
    Point2D* size;
    char* map;
    // 2d array [size.x][size.y]
public:
    Level(Point2D* _size, char* _map);
    Point2D* getMapSize();
    char* getMap();
};