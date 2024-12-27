#include "RoomConnected.h"

RoomConnected::RoomConnected(int _roomIndex, Point2D* _wallStart, Point2D* _wallEnd, int _weight)
{
    this->roomIndex = _roomIndex;
    this->wallStart = _wallStart;
    this->wallEnd = _wallEnd;
    this->weight = _weight;
}
RoomConnected::RoomConnected(int _roomIndex, int _wallStartPointX, int _wallStartPointY, int _wallEndPointX, int _wallEndPointY, int _weight)
{
    this->roomIndex = _roomIndex;
    this->wallStart = new Point2D(_wallStartPointX, _wallStartPointY);
    this->wallEnd = new Point2D(_wallEndPointX, _wallEndPointY);
    this->weight = _weight;
}
RoomConnected::RoomConnected(const RoomConnected* roomConnected)
{
    this->roomIndex = roomConnected->roomIndex;
    this->wallStart = new Point2D(roomConnected->wallStart);
    this->wallEnd = new Point2D(roomConnected->wallEnd);
    this->weight = roomConnected->weight;
}
RoomConnected::~RoomConnected() {}

Point2D* RoomConnected::getWallStart()
{
    return this->wallStart;
}
Point2D* RoomConnected::getWallEnd()
{
    return this->wallEnd;
}
int RoomConnected::getRoomIndex()
{
    return this->roomIndex;
}
void RoomConnected::setRoomIndex(int _roomIndex)
{
    this->roomIndex = _roomIndex;
}
int RoomConnected::getWeight()
{
    return this->weight;
}
int RoomConnected::getIsConnectComplete()
{
    return this->isConnectComplete;
}
void RoomConnected::setIsConnectComplete(int _isConnectComplete)
{
    this->isConnectComplete = _isConnectComplete;
}