#pragma once
#include "RoomConnected.h"
#include "Room.h"
namespace LevelGenerator
{
    class RoomConnection {
    private:
        Room* curRoom;
        // ������� �������
        RoomConnected* connectedRooms; // �������� ������ !!!
        int connectedCount = 0;
        // � ������ (������)
    public:
        RoomConnection(Room* _curRoom, RoomConnected* _connectedRooms, int _ConnectedCount);
        ~RoomConnection();
        Room* getCurRoom();
        void setCurRoom(Room* _curRoom);
        RoomConnected* getConnectedRooms();
        void setConnectedRooms(RoomConnected* _connectedRooms);
        int getConnectedCount();
        int IncrementConnectedCount();
        void setConnectedCount(int _connectedCount);
    };
}