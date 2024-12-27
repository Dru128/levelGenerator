#include "LevelGenerator.h"
#include "tools.h"
#include <iostream>



LevelGenerator::LevelGenerator() {
    srand(time(0));
    // инициализация генератора случайных чисел
};
LevelGenerator::~LevelGenerator() {};

void LevelGenerator::roomArrayToMap(RoomConnection* roomsGraph, int n, char* map, Point2D* mapSize)
{
    Point2D* walls = (Point2D*)calloc(n * n, sizeof(Point2D));
    // индексы соседей в массиве roomsGraph
    int wallsSize = 0;


    // заполняем все стенами
    for (int y = 0; y < mapSize->getY(); y++)
    {
        for (int x = 0; x < mapSize->getX(); x++)
        {
            *(map + y * mapSize->getX() + x) = WALL_TAG;
        }
    }

    int count = 0,
        * connectedDraw = (int*)calloc(n * n, sizeof(int));
    for (int i = 0; i < n; i++)
    {
        // заполняем выборочно пустым пространством (комнаты)
        for (int y = roomsGraph[i].getCurRoom()->getStartPoint()->getY(); y <= roomsGraph[i].getCurRoom()->getEndPoint()->getY(); y++)
        {
            for (int x = roomsGraph[i].getCurRoom()->getStartPoint()->getX(); x <= roomsGraph[i].getCurRoom()->getEndPoint()->getX(); x++)
            {
                *(map + y * mapSize->getX() + x) = EMPTY_TAG;
            }
        }


        for (int j = 0; j < roomsGraph[i].getConnectedCount(); j++)
        { // строим проходы между комнатами
            if (roomsGraph[i].getConnectedRooms()[j].getIsConnectComplete())
            {
               int isContainCurEdge = 0,
                   first,
                   second;
                if (i < roomsGraph[i].getConnectedRooms()[j].getRoomIndex()) {
                    first = i;
                    second = j;
                }
                else {
                    first = roomsGraph[i].getConnectedRooms()[j].getRoomIndex();
                    for (int k = 0; k < roomsGraph[first].getConnectedCount(); k++)
                        if (i == roomsGraph[first].getConnectedRooms()[k].getRoomIndex())
                        {
                            second = k;
                            break;
                        }
                }
                Point2D* curEdge = new Point2D(first, second); // ребро графа
                // индексы: roomsGraph[x].getConnectedRooms()[y]

                for (int k = 0; k < wallsSize && isContainCurEdge == 0; k++)
                    if (walls[k].getX() == curEdge->getX() && walls[k].getY() == curEdge->getY())
                        isContainCurEdge = 1;

                if (isContainCurEdge == 0)
                {
                    walls[wallsSize++] = curEdge;
                    int
                        randX = randomInt(
                            roomsGraph[curEdge->getX()].getConnectedRooms()[curEdge->getY()].getWallStart()->getX(),
                            roomsGraph[curEdge->getX()].getConnectedRooms()[curEdge->getY()].getWallEnd()->getX()
                        ),
                        randY = randomInt(
                            roomsGraph[curEdge->getX()].getConnectedRooms()[curEdge->getY()].getWallStart()->getY(),
                            roomsGraph[curEdge->getX()].getConnectedRooms()[curEdge->getY()].getWallEnd()->getY()
                        );
                    *(map + randY * mapSize->getX() + randX) = EMPTY_TAG;

                }
                else
                    delete (curEdge);
            }
        }
    }
}

Room* LevelGenerator::divideRooms(Room* rooms, int* n, LevelGenerParams* lvlGenParams)
{
    Room* _rooms = (Room*)malloc(2 * (*n) * sizeof(Room));
    int new_n = 0;

    for (int i = 0; i < (*n); i++)
    {
        int
            delta_x = rooms[i].getEndPoint()->getX() - rooms[i].getStartPoint()->getX(),
            delta_y = rooms[i].getEndPoint()->getY() - rooms[i].getStartPoint()->getY(),
            delta_max = std::max(delta_x, delta_y);

        rooms[i].setDivideComplete(
            delta_max < lvlGenParams->getMaxRoomSize() && (
                rooms[i].getIsdivideComplete() ||
                lvlGenParams->getProbabOfDivide() < randomInt(0, 100) ||
                delta_max < 2 * lvlGenParams->getMinRoomSize() + 1
                )
        );
        // Здесь мы решаем будем делить комнату или просто скопируем в новый массив
        // И пометим её как неделимую 

        if (rooms[i].getIsdivideComplete() == 0)
        {
            if (delta_x > delta_y)
            {
                // место разреза комнаты
                int divide = randomInt(
                    rooms[i].getStartPoint()->getX() + lvlGenParams->getMinRoomSize(),
                    rooms[i].getEndPoint()->getX() - lvlGenParams->getMinRoomSize()
                );
                // поделили на 2 комнаты по оси X
                _rooms[new_n++] = new Room(
                    rand(),
                    new Point2D(rooms[i].getStartPoint()->getX(), rooms[i].getStartPoint()->getY()),
                    new Point2D(divide - 1, rooms[i].getEndPoint()->getY()),
                    rooms[i].getIsdivideComplete()
                );
                _rooms[new_n++] = new Room(
                    rand(),
                    new Point2D(divide + 1, rooms[i].getStartPoint()->getY()),
                    new Point2D(rooms[i].getEndPoint()->getX(), rooms[i].getEndPoint()->getY()),
                    rooms[i].getIsdivideComplete()
                );
            }
            else
            {
                // место разреза комнаты
                int divide = randomInt(
                    rooms[i].getStartPoint()->getY() + lvlGenParams->getMinRoomSize(),
                    rooms[i].getEndPoint()->getY() - lvlGenParams->getMinRoomSize()
                );

                // поделили на 2 комнаты по оси Y
                _rooms[new_n++] = new Room(
                    rand(),
                    new Point2D(rooms[i].getStartPoint()->getX(), rooms[i].getStartPoint()->getY()),
                    new Point2D(rooms[i].getEndPoint()->getX(), divide - 1),
                    rooms[i].getIsdivideComplete()
                );
                _rooms[new_n++] = new Room(
                    rand(),
                    new Point2D(rooms[i].getStartPoint()->getX(), divide + 1),
                    new Point2D(rooms[i].getEndPoint()->getX(), rooms[i].getEndPoint()->getY()),
                    rooms[i].getIsdivideComplete()
                );
            }
        }
        else
        {
            // комнату поделить нельзя 
            // копируем ее в новый массив комнат
            _rooms[new_n++] = new Room(rooms[i]);
        }
    }

    free(rooms);
    if (new_n == (*n))
    {
        *n = new_n;
        return _rooms;
        // вернуть новый массив поделенных комнат
    }
    else
    {
        *n = new_n;
        divideRooms(_rooms, n, lvlGenParams);
        // повторяем еще раз 
    }
}


void LevelGenerator::makeRoomGraph(Room* rooms, int n, RoomConnection* roomsGraph)
{
    for (int i = 0; i < n; i++)
    {
        roomsGraph[i].setCurRoom(&rooms[i]);
        roomsGraph[i].setConnectedRooms((RoomConnected*)malloc(n * sizeof(RoomConnected)));
        roomsGraph[i].setConnectedCount(0);
    }

    for (int cur_i = 0; cur_i < n; cur_i++)
    {
        for (int i = cur_i; i < n; i++)
        {
            if (cur_i == i) continue;

            if (rooms[cur_i].getEndPoint()->getX() + 2 == rooms[i].getStartPoint()->getX())
            { // проверка правого края
                int startY, endY;
                if (commonInterval(
                    rooms[cur_i].getStartPoint()->getY(), rooms[cur_i].getEndPoint()->getY(),
                    rooms[i].getStartPoint()->getY(), rooms[i].getEndPoint()->getY(),
                    &startY, &endY))
                {
                    int weight = randomInt(0, 100);
                    roomsGraph[cur_i].getConnectedRooms()[roomsGraph[cur_i].IncrementConnectedCount()] = RoomConnected(
                        i,
                        new Point2D(rooms[cur_i].getEndPoint()->getX() + 1, startY),
                        new Point2D(rooms[cur_i].getEndPoint()->getX() + 1, endY),
                        weight
                    );
                    roomsGraph[i].getConnectedRooms()[roomsGraph[i].IncrementConnectedCount()] = RoomConnected(
                        cur_i,
                        new Point2D(rooms[i].getStartPoint()->getX() - 1, startY),
                        new Point2D(rooms[i].getStartPoint()->getX() - 1, endY),
                        weight
                    );
                }
                if (startY > endY)
                printf("%d %d\n", startY, endY);
            }
            else if (rooms[cur_i].getStartPoint()->getX() - 2 == rooms[i].getEndPoint()->getX())
            { // проверка левого края
                int startY, endY;
                if (commonInterval(
                    rooms[cur_i].getStartPoint()->getY(), rooms[cur_i].getEndPoint()->getY(),
                    rooms[i].getStartPoint()->getY(), rooms[i].getEndPoint()->getY(),
                    &startY, &endY))
                {
                    int weight = randomInt(0, 100);
                    roomsGraph[cur_i].getConnectedRooms()[roomsGraph[cur_i].IncrementConnectedCount()] = RoomConnected(
                        i,
                        new Point2D(rooms[cur_i].getStartPoint()->getX() - 1, startY),
                        new Point2D(rooms[cur_i].getStartPoint()->getX() - 1, endY),
                        weight
                    );
                    roomsGraph[i].getConnectedRooms()[roomsGraph[i].IncrementConnectedCount()] = RoomConnected(
                        cur_i,
                        new Point2D(rooms[i].getEndPoint()->getX() + 1, startY),
                        new Point2D(rooms[i].getEndPoint()->getX() + 1, endY),
                        weight
                    );
                }
            }
            else if (rooms[cur_i].getEndPoint()->getY() + 2 == rooms[i].getStartPoint()->getY())
            { // проверка верхнего края
                int startX, endX;
                if (commonInterval(
                    rooms[cur_i].getStartPoint()->getX(), rooms[cur_i].getEndPoint()->getX(),
                    rooms[i].getStartPoint()->getX(), rooms[i].getEndPoint()->getX(),
                    &startX, &endX))
                {
                    int weight = randomInt(0, 100);
                    roomsGraph[cur_i].getConnectedRooms()[roomsGraph[cur_i].IncrementConnectedCount()] = RoomConnected(
                        i,
                        new Point2D(startX, rooms[cur_i].getEndPoint()->getY() + 1),
                        new Point2D(endX, rooms[cur_i].getEndPoint()->getY() + 1),
                        weight
                    );
                    roomsGraph[i].getConnectedRooms()[roomsGraph[i].IncrementConnectedCount()] = RoomConnected(
                        cur_i,
                        new Point2D(startX, rooms[i].getStartPoint()->getY() - 1),
                        new Point2D(endX, rooms[i].getStartPoint()->getY() - 1),
                        weight
                    );
                }
            }
            else if (rooms[cur_i].getStartPoint()->getY() - 2 == rooms[i].getEndPoint()->getY())
            { // проверка нижнего края
                int startX, endX;
                if (commonInterval(
                    rooms[cur_i].getStartPoint()->getX(), rooms[cur_i].getEndPoint()->getX(),
                    rooms[i].getStartPoint()->getX(), rooms[i].getEndPoint()->getX(),
                    &startX, &endX))
                {
                    int weight = randomInt(0, 100);
                    roomsGraph[cur_i].getConnectedRooms()[roomsGraph[cur_i].IncrementConnectedCount()] = RoomConnected(
                        i,
                        new Point2D(startX, rooms[cur_i].getStartPoint()->getY() - 1),
                        new Point2D(endX, rooms[cur_i].getStartPoint()->getY() - 1),
                        weight
                    );
                    roomsGraph[i].getConnectedRooms()[roomsGraph[i].IncrementConnectedCount()] = RoomConnected(
                        cur_i,
                        new Point2D(startX, rooms[i].getEndPoint()->getY() + 1),
                        new Point2D(endX, rooms[i].getEndPoint()->getY() + 1),
                        weight
                    );
                }
            }

        }
    }
}

void LevelGenerator::makeRoomConnections(int n, RoomConnection* roomsGraph)
{
    int nLeaves = 1,
        * leaves = (int*)calloc(n, sizeof(int));
    // нужно выбрать первый лист и выделить память
    leaves[0] = 0;


    RoomConnected* minWeightRoom = NULL;
    int isInitMinWeight,
        targetLeaveIndex;
    int p = n;
    while (nLeaves != p)
    { // присоединяем пока листов не будет = вершинам (комнатам)
        isInitMinWeight = 0;
        for (int il = 0; il < nLeaves; il++)
        { // перебираем листы

            int conCount = roomsGraph[leaves[il]].getConnectedCount();
            for (int i = 0; i < conCount; i++)
            { // перебираем соседей листов
                if (roomsGraph[leaves[il]].getConnectedRooms()[i].getIsConnectComplete() == 0)
                { // соединения еще нет
                    int isNotLeaf = 1;
                    for (int _il = 0; _il < nLeaves && isNotLeaf; _il++)
                        isNotLeaf = leaves[_il] != roomsGraph[leaves[il]].getConnectedRooms()[i].getRoomIndex();

                    if (isNotLeaf)
                    { // это не два листа
                        if (isInitMinWeight)
                        {
                            if (roomsGraph[leaves[il]].getConnectedRooms()[i].getWeight() < minWeightRoom->getWeight())
                            {
                                isInitMinWeight = 1;
                                minWeightRoom = &roomsGraph[leaves[il]].getConnectedRooms()[i];
                                targetLeaveIndex = il;
                            }
                        }
                        else
                        {
                            isInitMinWeight = 1;
                            minWeightRoom = &roomsGraph[leaves[il]].getConnectedRooms()[i];
                            targetLeaveIndex = il;
                        }
                    }
                }
            }
        }

        // мы выбрали вершину графа (комнату), 
        // которая граничит с одним из листов, 
        // не является листом и обладает минимальным весом
        if (isInitMinWeight == 1)
        {
            leaves[nLeaves] = minWeightRoom->getRoomIndex();
            nLeaves++;
            int firstI = leaves[targetLeaveIndex],
                secondI = minWeightRoom->getRoomIndex();



            for (int k = 0; k < roomsGraph[firstI].getConnectedCount(); k++)
                if (roomsGraph[firstI].getConnectedRooms()[k].getRoomIndex() == secondI)
                {
                    roomsGraph[firstI].getConnectedRooms()[k].setIsConnectComplete(1);
                    break;
                }

            for (int k = 0; k < roomsGraph[secondI].getConnectedCount(); k++)
                if (roomsGraph[secondI].getConnectedRooms()[k].getRoomIndex() == firstI)
                {
                    roomsGraph[secondI].getConnectedRooms()[k].setIsConnectComplete(1);
                    break;
                }
        }
        else p--;
            //printf("MinWeightRoom not init!");
    }
}
void LevelGenerator::startGeneration(Level* level, LevelGenerParams* lvlGenParams)
{
    Room* rooms = (Room*)malloc(sizeof(Room));

    // массив комнат
    int n = 1;
    rooms[0] = Room(
        rand(),
        new Point2D(1, 1),
        new Point2D(level->getMapSize()->getX() - 2, level->getMapSize()->getY() - 2),
        0
    );
    rooms = divideRooms(rooms, &n, lvlGenParams);
    // делим на комнаты

    RoomConnection* roomsGraph = (RoomConnection*)malloc(n * sizeof(RoomConnection));
    makeRoomGraph(rooms, n, roomsGraph);
    // построение графа комнат-соседей (список смежности)
    // каждая комната харнит индексы в исходном массиве своих комнат-соседей

    makeRoomConnections(n, roomsGraph);
    // Алгоритм Прима - строим минимальное остовное дерево взвешенного связного неориентированного графа,
    // где веса ребер - веса связей комнат RoomConnected.weight
    roomArrayToMap(roomsGraph, n, level->getMap(), level->getMapSize());
    // инициализируем level.map

    free(rooms);
    free(roomsGraph);
}