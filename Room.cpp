#include "Room.h"

Room::Room(int _id, Point2D* _startPoint, Point2D* _endPoint, int _isdivideComplete)
{
    this->id = _id;
    this->startPoint = _startPoint;
    this->endPoint = _endPoint;
    this->isdivideComplete = _isdivideComplete;
}
Room::Room(int _id, int _startPointX, int _startPointY, int _endPointX, int _EndPointY, int _isdivideComplete)
{
    this->id = _id;
    this->startPoint = new Point2D(_startPointX, _startPointY);
    this->endPoint = new Point2D(_endPointX, _EndPointY);
    this->isdivideComplete = _isdivideComplete;
}
Room::Room(const Room* room)
{
    this->id = room->id + 1;
    this->startPoint = new Point2D(room->startPoint);
    this->endPoint = new Point2D(room->endPoint);
    this->isdivideComplete = room->isdivideComplete;
}
Room::~Room() {}
Point2D* Room::getStartPoint()
{
    return startPoint;
}
Point2D* Room::getEndPoint()
{
    return endPoint;
}
int Room::getId()
{
    return this->id;
}
int Room::getIsdivideComplete()
{
    return this->isdivideComplete;
}
void Room::setDivideComplete(int _isdivideComplete)
{
    this->isdivideComplete = _isdivideComplete;
}
