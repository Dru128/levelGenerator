#include "RoomConnection.h"


RoomConnection::RoomConnection(Room* _curRoom, RoomConnected* _connectedRooms, int _ConnectedCount)
{
    this->curRoom = _curRoom;
    this->connectedRooms = _connectedRooms;
    this->connectedCount = _ConnectedCount;
}
RoomConnection::~RoomConnection() {}
Room* RoomConnection::getCurRoom()
{
    return this->curRoom;
}
void RoomConnection::setCurRoom(Room* _curRoom)
{
    this->curRoom = _curRoom;
}
RoomConnected* RoomConnection::getConnectedRooms()
{
    return this->connectedRooms;
}
void RoomConnection::setConnectedRooms(RoomConnected* _connectedRooms)
{
    this->connectedRooms = _connectedRooms;
}
int RoomConnection::getConnectedCount()
{
    return this->connectedCount;
}
int RoomConnection::IncrementConnectedCount()
{
    return this->connectedCount++;
}
void RoomConnection::setConnectedCount(int _connectedCount)
{
    this->connectedCount = _connectedCount;
}
