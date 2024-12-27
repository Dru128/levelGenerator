#pragma once
#include "Point2D.h"
/*
* start = ����� ������ ����
* end = ������ ������� ����
*/
class Room {
private:
    int id;
    Point2D* startPoint, * endPoint;
    int isdivideComplete = 0;
    // ������� ���������� - ����� �� ������� �������� ���
public:
    Room(int _id, Point2D* _startPoint, Point2D* _endPoint, int _isdivideComplete);
    Room(int _id, int _startPointX, int _startPointY, int _endPointX, int _EndPointY, int _isdivideComplete);
    Room(const Room* room);
    ~Room();
    int getId();
    Point2D* getStartPoint();
    Point2D* getEndPoint();
    int getIsdivideComplete();
    void setDivideComplete(int _isdivideComplete);
};