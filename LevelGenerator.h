#pragma once

#define EMPTY_TAG '0'
#define WALL_TAG '1'
#define START_TAG '4'
#define FINISH_TAG '5'
#define ENEMY_TAG '6'
#include "LevelGenerParams.h"
#include "Room.h"
#include "RoomConnection.h"
#include "Level.h"
namespace LevelGenerator
{
    class LevelGenerator {
    public:
        void startGeneration(Level* level, LevelGenerParams* lvlGenParams);
        LevelGenerator();
        ~LevelGenerator();
    private:
        Room* divideRooms(Room* rooms, int* n, LevelGenerParams* lvlGenParams);
        void makeRoomGraph(Room* rooms, int n, RoomConnection* roomsGraph);
        void makeRoomConnections(int n, RoomConnection* roomsGraph);
        void roomArrayToMap(RoomConnection* roomsGraph, int n, char* map, Point2D* mapSize);
    };
}