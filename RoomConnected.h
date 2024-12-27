#pragma once
#include "Point2D.h"

class RoomConnected {
private:
    int roomIndex;
    // индекс комнаты в исходном массиве

    Point2D* wallStart, * wallEnd;
    // начало и конец границы комнат
    int weight; // вес ребра
    int isConnectComplete = 0;
public:
    RoomConnected(int _roomIndex, Point2D* _wallStart, Point2D* _wallEnd, int _weight);
    RoomConnected(int _roomIndex, int _wallStartPointX, int _wallStartPointY, int _wallEndPointX, int _wallEndPointY, int _weight);
    RoomConnected(const RoomConnected* roomConnected);
    ~RoomConnected();
    Point2D* getWallStart();
    Point2D* getWallEnd();
    int getRoomIndex();
    void setRoomIndex(int _roomIndex);
    int getWeight();
    int getIsConnectComplete();
    void setIsConnectComplete(int _isConnectComplete);
};