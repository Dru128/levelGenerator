// levelGenerator.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <random>
#include <string>
#include <list>

#define EMPTY_TAG '1'
#define WALL_TAG '2'
#define START_TAG '4'
#define FINISH_TAG '5'
#define ENEMY_TAG '6'

class IPoint {
public:
    virtual ~IPoint() = default;
    virtual float distanceToNull() = 0;
    virtual void printPoint() = 0;
};

class Point2D: public IPoint {
protected:
    int x, y;
public:
    Point2D(int _x, int _y);
    Point2D(const Point2D* point);
    ~Point2D() = default;
    int getX();
    void setX(int _x);
    int getY();
    void setY(int _y);
    float distanceToNull() override {
        return sqrt(x * x + y * y);
    };  
    void printPoint() override {
        printf("x = %d\n", x);
        printf("y = %d\n", y);
    }
    int operator == (Point2D* point)
    {
        return this->x == point->getX() && this->y == point->getY();
    }
    Point2D* operator + (Point2D* point)
    {
        return new Point2D(this->x + point->getX(), this->y + point->getY());
    }
    Point2D* operator - (Point2D* point) const
    {
        return new Point2D(this->x - point->getX(), this->y - point->getY());
    }
    Point2D* operator + (int num) const
    {
        return new Point2D(this->x + num, this->y + num);
    }
    Point2D* operator - (int num) const
    {
        return new Point2D(this->x - num, this->y - num);
    }
    Point2D* operator * (int num) const
    {
        return new Point2D(this->x * num, this->y * num);
    }
};
Point2D::Point2D(int _x, int _y)
{
    this->x = _x;
    this->y = _y;
}
Point2D::Point2D(const Point2D* point)
{
    this->x = point->x;
    this->y = point->y;
}
int Point2D::getX()
{
    return this->x;
}
void Point2D::setX(int _x)
{
    this->x = _x;
}
int Point2D::getY()
{
    return this->y;
}
void Point2D::setY(int _y)
{
    this->y = _y;
}

class Point3D final : public Point2D
{
protected:
    int z;
public:
    Point3D(int x, int y, int z);
    Point3D(Point2D* point, int z);
    Point3D(const Point3D* point);
    ~Point3D();
    int getZ();
    void setZ(int z);
    float distanceToNull() override {
        return sqrt(x * x + y * y + z * z);
    };
    void printPoint() override {
        Point2D::printPoint();
        printf("z = %d\n", z);
    }
    Point3D* operator + (Point3D* point) const
    {
        return new Point3D(this->x + point->getX(), this->y + point->getY(), this->z + point->getZ());
    }
    Point3D* operator - (Point3D* point) const
    {
        return new Point3D(this->x - point->getX(), this->y - point->getY(), this->z - point->getZ());
    }
    Point3D* operator + (int num) const
    {
        return new Point3D(this->x + num, this->y + num, this->z + num);
    }
    Point3D* operator - (int num) const
    {
        return new Point3D(this->x - num, this->y - num, this->z - num);
    }
    Point3D* operator * (int num) const
    {
        return new Point3D(this->x * num, this->y * num, this->z * num);
    }
};
Point3D::Point3D(int x, int y, int z) : Point2D(x, y)
{
    this->z = z;
}
Point3D::Point3D(Point2D* point, int z) : Point2D(point)
{
    this->z = z;
}
Point3D::Point3D(const Point3D* point): Point2D(point)
{
    this->z = point->z;
}
Point3D::~Point3D()
{
    // clear data
}
int Point3D::getZ()
{
    return this->z;
}
void Point3D::setZ(int z)
{
    this->z = z;
}



bool compareByDistance(const Point2D& left, const Point2D& right)
{
    return left.distanceToNull() > right.distanceToNull();
}
int main()
{

    std::list<Point2D> pointList{
        new Point2D(1, 1),
        new Point2D(3, 4),
        new Point2D(1, 10),
        new Point3D(1, 1, 1),
        new Point3D(3, 0, 10),
        new Point2D(8, 0),
        new Point2D(1, 10),
        new Point3D(0, 3, 4)
    };
    for (auto iter = pointList.begin(); iter != pointList.end(); iter++)
    {
        printf("x = %d, y = %d\n", iter->getX(), iter->getY());
    }
    // нашли первый элемент с такими координатами
    std::list<Point2D>::iterator findIter = std::find(pointList.begin(), pointList.end(), new Point2D(1, 10));
    printf("\nfind: x = %d, y = %d\n", findIter->getX(), findIter->getY());

    // отсортированы по расстоянию к началу координат
    std::sort(pointList.begin(), pointList.end(), compareByDistance);

    /*Point2D* point2d = new Point2D(3, 4)
    Point3D* point3d = new Point3D(point2d, 5);

    printf("parent virtual: \n");
    point2d->printPoint();

    printf("child virtual: \n");
    point3d->printPoint();\

    printf("child perform parent virtual: \n");
    ((Point2D)point3d).printPoint();

    Point2D* point2d_2 = new Point2D(point2d);
    point2d->setY(10);
    Point2D* point2d_3 = point2d->operator+(point2d_2);
    Point2D* point2d_4 = point2d + 10;*/
}


// данные уровня
class Level {
private:
    Point2D* size;
    char* map;
    // 2d array [size.x][size.y]
public:
    Level(Point2D* _size , char* _map);
    Point2D* getMapSize();
    char* getMap();
};

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

// параметры генерации уровня
class LevelGenerParams {
private:
    int
        minRoomSize,
        maxRoomSize,
        probabOfDivide;
    // вероятность деления комнаты [0..100]
public:
    LevelGenerParams(int _minRoomSize, int _maxRoomSize, int _probabOfDivide);
    int getMinRoomSize();
    int getMaxRoomSize();
    int getProbabOfDivide();
};
LevelGenerParams::LevelGenerParams(int _minRoomSize, int _maxRoomSize, int _probabOfDivide)
{
    this->minRoomSize = _minRoomSize;
    this->maxRoomSize = _maxRoomSize;
    this->probabOfDivide = _probabOfDivide;
}
int LevelGenerParams::getMinRoomSize()
{
    return this->minRoomSize;
}
int LevelGenerParams::getMaxRoomSize()
{
    return this->maxRoomSize;
}
int LevelGenerParams::getProbabOfDivide()
{
    return this->probabOfDivide;
}

/*
* start = левый нижний угол 
* end = правый верхний угол 
*/
class Room  {
private:
    int id;
    Point2D* startPoint, *endPoint;
    int isdivideComplete = 0;
    // булевая переменная - будет ли комната делиться ещё
public:
    Room(int _id, Point2D* _startPoint, Point2D* _endPoint, int _isdivideComplete);
    Room(int _id, int _startPointX, int _startPointY, int _endPointX, int _EndPointY, int _isdivideComplete);
    Room(const Room* room);
    int getId();
    Point2D* getStartPoint();
    Point2D* getEndPoint();
    int getIsdivideComplete();
    void setDivideComplete(int _isdivideComplete);
};
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

class RoomConnected {
private:
    int roomIndex;
    // индекс комнаты в исходном массиве

    Point2D *wallStart, *wallEnd;
    // начало и конец границы комнат
    int weight; // вес ребра
    int isConnectComplete = 0;
public:
    RoomConnected(int _roomIndex, Point2D* _wallStart, Point2D* _wallEnd, int _weight);
    RoomConnected(int _roomIndex, int _wallStartPointX, int _wallStartPointY, int _wallEndPointX, int _wallEndPointY, int _weight);
    RoomConnected(const RoomConnected* roomConnected);
    Point2D* getWallStart();
    Point2D* getWallEnd();
    int getRoomIndex();
    void setRoomIndex(int _roomIndex);
    int getWeight();
    int getIsConnectComplete();
    void setIsConnectComplete(int _isConnectComplete);
};
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



class RoomConnection {
private:
    Room* curRoom;
    // текущая комната
    RoomConnected* connectedRooms; // выделить память !!!
    int connectedCount = 0;
    // её соседи (массив)
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
RoomConnection::RoomConnection(Room* _curRoom, RoomConnected* _connectedRooms, int _ConnectedCount)
{
    this->curRoom = _curRoom;
    this->connectedRooms = _connectedRooms;
    this->connectedCount = _ConnectedCount;
}
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
RoomConnection::~RoomConnection()
{

}



int randomInt(int _min, int _max)
{
    int r = rand() % (_max - _min + 1) + _min;
    return r;
}

int& maxValue(int& a, int& b)
{
    if (a > b)
        return a;
    else 
        return b;
}

// -----------------< graphic >----------------------
// отрисовка карты
void drawMap(const char* map, Point2D player, int countEnemies, Point2D mapSize)
{
    //system("CLS"); // выдает ошибку
    //clearScreen();
    int isDrawCharacter = 0;

    for (int y = 0; y < mapSize.getY(); y++)
    {
        for (int x = 0; x < mapSize.getX(); x++)
        {
            //if (x >= WIDTH || x >= WIDTH) return;
            isDrawCharacter = 0;
            if (player.getX() == x && player.getY() == y)
            {
                putchar(1); putchar(' '); isDrawCharacter = 1;
            }

            /*if (isDrawCharacter == 0)
                for (int i = 0; i < countEnemies; i++)
                    if ((*(enemies + i)).x == x && (*(enemies + i)).y == y)
                    {
                        putchar('}'); putchar('{'); isDrawCharacter = 1;
                    }*/
            if (isDrawCharacter == 0)
                switch (*(map + y * mapSize.getX() + x))
                {
                case EMPTY_TAG: {
                    putchar(' '); putchar(' ');
                    break;
                }
                case WALL_TAG: {
                    putchar(219); putchar(219);
                    break;
                }
                case ENEMY_TAG: {
                    putchar(' '); putchar(' ');
                    break;
                }
                case START_TAG: {
                    putchar(' '); putchar(' ');
                    break;
                }
                case FINISH_TAG: {
                    putchar(177); putchar(177);
                    break;
                }
                default: {
                    putchar('*'); putchar('*');
                    break;
                }
                }
        }
        putchar('\n');
    }
}
Point2D* getConsoleSize()
{
    Point2D* size = new Point2D(30, 60);
    system("mode con cols=120 lines=30");

    HANDLE hWndConsole;
    if (hWndConsole = GetStdHandle(-12))
    {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
        {
            size->setX((consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1) / 2);
            size->setY(consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1);
            return size;
        }
        else
        {
            printf("Error: %d\n", GetLastError());
        }
    }
    else
    {
        printf("Error: %d\n", GetLastError());
    }
    getchar();

    return size;
}


// выгрузка карты из базы данных
/*int loadMapFromFile(Level* level)
{
    FILE* levelDB;
    levelDB = fopen("level_1.txt", "r");
    if (levelDB == NULL) return 0;
    char symbol;

    rewind(levelDB);
    for (int y = 0; y < level->getMapSize()->getY(); y++)
        for (int x = 0; x <= level->getMapSize()->getX(); x++)
        {
            symbol = getc(levelDB);
            //putchar(symbol);
            if (symbol == EOF) { return 0; }
            else if (symbol == NULL) { return 0; }
            else if (symbol == '\n') { break; }
            else
            {
                *(level->getMap() + y * level->getMapSize()->getX() + x) = symbol;
            }
        }

    fclose(levelDB);
}*/

class LevelGenerator {

public:
    void startGeneration(Level* level, LevelGenerParams* lvlGenParams);
    LevelGenerator();
    ~LevelGenerator();
private:
    void roomArrayToMap(RoomConnection* roomsGraph, int n, char* map, Point2D* mapSize);
    Room* divideRooms(Room* rooms, int* n, LevelGenerParams* lvlGenParams);
    void makeRoomGraph(Room* rooms, int n, RoomConnection* roomsGraph);
    void makeRoomConnections(int n, RoomConnection* roomsGraph);

};
LevelGenerator::LevelGenerator() {};
LevelGenerator::~LevelGenerator() {};

void LevelGenerator::roomArrayToMap(RoomConnection* roomsGraph, int n, char* map, Point2D *mapSize)
{
    // заполняем все стенами
    for (int y = 0; y < mapSize->getX(); y++)
    {
        for (int x = 0; x < mapSize->getX(); x++)
        {
            *(map + y * mapSize->getX() + x) = WALL_TAG;
        }
    }

    int count = 0,
        * connectedDraw = (int*)calloc(n*n, sizeof(int));
    // заполняем выборочно пустым пространством (комнаты)
    for (int i = 0; i < n; i++)
    {
        for (int y = roomsGraph[i].getCurRoom()->getStartPoint()->getY(); y <= roomsGraph[i].getCurRoom()->getEndPoint()->getY(); y++)
        {
            for (int x = roomsGraph[i].getCurRoom()->getStartPoint()->getX(); x <= roomsGraph[i].getCurRoom()->getEndPoint()->getX(); x++)
            {
                *(map + y * mapSize->getX() + x) = EMPTY_TAG;
            }
        }
        // TODO
        for (int j = 0; j < roomsGraph[i].getConnectedCount() && roomsGraph[i].getConnectedRooms()[j].getIsConnectComplete(); j++)
        {
            // исправить два прохода на один!
            int randX = randomInt(
                roomsGraph[i].getConnectedRooms()[j].getWallStart()->getX(),
                roomsGraph[i].getConnectedRooms()[j].getWallStart()->getX()
                //roomsGraph[i].getConnectedRooms()[j].getWallEnd()->getX()
                ),
                randY = randomInt(
                    roomsGraph[i].getConnectedRooms()[j].getWallStart()->getY(),
                    roomsGraph[i].getConnectedRooms()[j].getWallStart()->getY()
                    //roomsGraph[i].getConnectedRooms()[j].getWallEnd()->getY()
                );
            *(map + randY * mapSize->getX() + randX) = EMPTY_TAG;
            /*if (roomsGraph[i].connectedRooms[j].isConnectComplete)
            {
                for (int y = roomsGraph[i].connectedRooms[j].wallStart.y; y <= roomsGraph[i].connectedRooms[j].wallEnd.y; y++)
                {
                    for (int x = roomsGraph[i].connectedRooms[j].wallStart.x; x <= roomsGraph[i].connectedRooms[j].wallEnd.x; x++)
                    {
                        *(map + y * mapSize.x + x) = EMPTY_TAG;
                    }
                }
            }*/
        }
    }

}

Room* LevelGenerator::divideRooms(Room* rooms, int *n, LevelGenerParams* lvlGenParams)
{
    Room* _rooms = (Room*)malloc(2 * (*n) * sizeof(Room));
    int new_n = 0;

    for (int i = 0; i < (*n); i++)
    {
        printf("i = %d) [%d][%d] | [%d][%d]\n", i,
            rooms[i].getStartPoint()->getX(), rooms[i].getStartPoint()->getY(),
            rooms[i].getEndPoint()->getX(), rooms[i].getEndPoint()->getY());


        int
            delta_x = rooms[i].getEndPoint()->getX() - rooms[i].getStartPoint()->getX(),
            delta_y = rooms[i].getEndPoint()->getY() - rooms[i].getStartPoint()->getY(),
            delta_max = maxValue(delta_x, delta_y);
        
        rooms[i].setDivideComplete(
            lvlGenParams->getMaxRoomSize() > delta_max &&
            (
             rooms[i].getIsdivideComplete() ||
             lvlGenParams->getProbabOfDivide() < randomInt(0, 100) ||
             delta_max < 2 * lvlGenParams->getMinRoomSize()
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
                _rooms[new_n++] = Room (
                    rand(),
                    new Point2D( rooms[i].getStartPoint()->getX(), rooms[i].getStartPoint()->getY()),
                    new Point2D( divide - 1, rooms[i].getEndPoint()->getY()),
                    rooms[i].getIsdivideComplete()
                );
                _rooms[new_n++] = Room (
                    rand(),
                    new Point2D( divide + 1, rooms[i].getStartPoint()->getY()),
                    new Point2D( rooms[i].getEndPoint()->getX(), rooms[i].getEndPoint()->getY()),
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
                _rooms[new_n++] = Room (
                    rand(),
                    new Point2D(rooms[i].getStartPoint()->getX(), rooms[i].getStartPoint()->getY()),
                    new Point2D( rooms[i].getEndPoint()->getX(), divide - 1),
                    rooms[i].getIsdivideComplete()
                );
                _rooms[new_n++] = Room (
                    rand(),
                    new Point2D(rooms[i].getStartPoint()->getX(), divide + 1 ),
                    new Point2D(rooms[i].getEndPoint()->getX(), rooms[i].getEndPoint()->getY()),
                    rooms[i].getIsdivideComplete()
                );
            }
        }
        else 
        {
            // комнату поделить нельзя 
            // копируем ее в новый массив комнат
            _rooms[new_n++] = Room (
                rand(),
                new Point2D( rooms[i].getStartPoint()->getX(), rooms[i].getStartPoint()->getY()),
                new Point2D( rooms[i].getEndPoint()->getX(), rooms[i].getEndPoint()->getY()),
                rooms[i].getIsdivideComplete()
            );
        }
    }

    printf("\n\n");
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
// поиск общего подотрезка на пересечении двух отрезков
int commonInterval(int start1, int end1, int start2, int end2, int* commonStart, int* commonEnd)
{
    if (end1 >= start2 && start1 <= end2)
    { // проверка пересекаются ли отрезки
        
        *commonStart = maxValue(start1, start2);
        *commonEnd = maxValue(end1, end2);
        // общий отрезок

        return 1;
    }
    else
        return 0; // отрезки не пересекаются
}

void LevelGenerator::makeRoomGraph(Room* rooms, int n, RoomConnection* roomsGraph)
{ 
    for (int i = 0; i < n; i++)
    {
        roomsGraph[i].setCurRoom(&rooms[i]);
        roomsGraph[i].setConnectedRooms( (RoomConnected*)malloc(n * sizeof(RoomConnected)) );
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
                        new Point2D ( rooms[cur_i].getEndPoint()->getX() + 1, startY ),
                        new Point2D (rooms[cur_i].getEndPoint()->getX() + 1, endY ),
                        weight
                    );
                    roomsGraph[i].getConnectedRooms()[roomsGraph[i].IncrementConnectedCount()] = RoomConnected(
                        cur_i,
                        new Point2D (  rooms[i].getStartPoint()->getX() - 1, startY ),
                        new Point2D (  rooms[i].getStartPoint()->getX() - 1, endY ),
                        weight
                    );
                }
            } else if (rooms[cur_i].getStartPoint()->getX() - 2 == rooms[i].getEndPoint()->getX())
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
                        new Point2D( rooms[cur_i].getStartPoint()->getX() - 1, startY ),
                        new Point2D( rooms[cur_i].getStartPoint()->getX() - 1, endY ),
                        weight
                    );
                    roomsGraph[i].getConnectedRooms()[roomsGraph[i].IncrementConnectedCount()] = RoomConnected(
                        cur_i,
                        new Point2D( rooms[i].getEndPoint()->getX() + 1, startY ),
                        new Point2D( rooms[i].getEndPoint()->getX() + 1, endY ),
                        weight
                    );
                }
            } else if (rooms[cur_i].getEndPoint()->getY() + 2 == rooms[i].getStartPoint()->getY())
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
                        new Point2D( startX, rooms[cur_i].getEndPoint()->getY() + 1 ),
                        new Point2D( endX, rooms[cur_i].getEndPoint()->getY() + 1 ),
                        weight
                    );
                    roomsGraph[i].getConnectedRooms()[roomsGraph[i].IncrementConnectedCount()] = RoomConnected(
                        cur_i,
                        new Point2D( startX, rooms[i].getStartPoint()->getY() - 1 ),
                        new Point2D( endX, rooms[i].getStartPoint()->getY() - 1 ),
                        weight
                    );
                }
            } else if (rooms[cur_i].getStartPoint()->getY() - 2 == rooms[i].getEndPoint()->getY())
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
                        new Point2D( startX, rooms[cur_i].getStartPoint()->getY() - 1 ),
                        new Point2D( endX, rooms[cur_i].getStartPoint()->getY() - 1 ),
                        weight
                    );
                    roomsGraph[i].getConnectedRooms()[roomsGraph[i].IncrementConnectedCount()] = RoomConnected(
                        cur_i,
                        new Point2D( startX, rooms[i].getEndPoint()->getY() + 1 ),
                        new Point2D( endX, rooms[i].getEndPoint()->getY() + 1 ),
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
        *leaves = (int*)calloc(n, sizeof(int));
    // нужно выбрать первый лист и выделить память
    leaves[0] = 1;
    

    RoomConnected* minWeightRoom = NULL;
    int isInitMinWeight = 0, targetLeaveIndex;
    while (nLeaves != n)
    { // присоединяем пока листов не будет = вершинам (комнатам)
        isInitMinWeight = 0;
        for (int il = 0; il < nLeaves; il++)
        { // перебираем листы

            int conCount = roomsGraph[leaves[il]].getConnectedCount();
            for (int i = 0; i < conCount; i++)
            { // перебираем соседей листов
                if (roomsGraph[leaves[il]].getConnectedRooms()[i].getIsConnectComplete() == 0)
                { // соединения еще нет (1)
                    int flag = 1;
                    for (int _il = 0; _il < nLeaves && flag; _il++)
                        flag = leaves[_il] != roomsGraph[leaves[il]].getConnectedRooms()[i].getRoomIndex();
                    // это не два листа (2)
                   /* if (flag)
                    {
                        if (isInitMinWeight == 0)
                        {
                            isInitMinWeight = 1;
                            minWeightRoom = &roomsGraph[leaves[il]].connectedRooms[i];
                            targetLeaveIndex = il;
                        }
                    }*/

                    printf("leaf = %d, second = %d, weight = %d \n", leaves[il], roomsGraph[leaves[il]].getConnectedRooms()[i].getRoomIndex(),roomsGraph[leaves[il]].getConnectedRooms()[i].getWeight());
                    if ((isInitMinWeight == 0 || // инициализируем первым значением
                        roomsGraph[leaves[il]].getConnectedRooms()[i].getWeight() < minWeightRoom->getWeight()) && flag)
                    {
                        isInitMinWeight = 1;
                        minWeightRoom = &roomsGraph[leaves[il]].getConnectedRooms()[i];
                        targetLeaveIndex = il;
                    }

                }
            }
        }

        // мы выбрали вершину графа (комнату), 
        // которая граничит с одним из листов, 
        // не является листом и обладает минимальным весом
        printf("nleaf = %d, isInit = %d, targetI = %d, roomIndex = %d\n", nLeaves, isInitMinWeight, leaves[targetLeaveIndex], minWeightRoom->getRoomIndex());
        if (isInitMinWeight == 1)
        {
            leaves[nLeaves] = minWeightRoom->getRoomIndex();
            nLeaves++;


            for (int k = 0; k < roomsGraph[leaves[targetLeaveIndex]].getConnectedCount(); k++)
                if (roomsGraph[leaves[targetLeaveIndex]].getConnectedRooms()[k].getRoomIndex() == minWeightRoom->getRoomIndex())
                {
                    roomsGraph[leaves[targetLeaveIndex]].getConnectedRooms()[k].setIsConnectComplete(1);
                    printf("i = %d", k);

                    break;
                }

            for (int k = 0; k < roomsGraph[minWeightRoom->getRoomIndex()].getConnectedCount(); k++)
                if (roomsGraph[minWeightRoom->getRoomIndex()].getConnectedRooms()[k].getRoomIndex() == leaves[targetLeaveIndex])
                {
                    roomsGraph[minWeightRoom->getRoomIndex()].getConnectedRooms()[k].setIsConnectComplete(1);
                    printf(" j = %d\n", k);

                    break;
                }
        }
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

    delete(rooms);
    delete(roomsGraph);
}
int generateLevel(Level* level)
{

    Point2D* mapSize = getConsoleSize();

    level = new Level(mapSize, (char*)malloc(mapSize->getX() * mapSize->getY() * sizeof(char)));
    LevelGenerParams* lvlGenParams;
    std::string input;
    int minRoomSize, maxRoomSize, probabOfDivide;
input_minRoomSize:
    try {
        std::cout << "Input min rooms size: ";
        getline(std::cin, input);
        minRoomSize = std::stoi(input);
    }
    catch (std::exception& e) {
        goto input_minRoomSize;
    }
input_maxRoomSize:
    try {
        std::cout << "Input max rooms size: ";
        getline(std::cin, input);
        maxRoomSize = std::stoi(input);
    }
    catch (std::exception& e) {
        goto input_maxRoomSize;
    }
input_probabOfDivide:
    try {
        std::cout << "Input probab of divide room: ";
        getline(std::cin, input);
        probabOfDivide = std::stoi(input);
    }
    catch (std::exception& e) {
        goto input_probabOfDivide;
    }
    lvlGenParams = new LevelGenerParams(minRoomSize, maxRoomSize, probabOfDivide);
    std::cout << "min room size = " + std::to_string(minRoomSize) + "\n" +
        "max room size = " + std::to_string(maxRoomSize) + "\n" +
        "probab of divide = " + std::to_string(probabOfDivide) << std::endl;

    
    LevelGenerator* generator = new LevelGenerator();
    generator->startGeneration(level, lvlGenParams);
    return 1; // Success

}


//int main()
//{
//    srand(time(0));
//    // инициализация генератора случайных чисел
//    Point2D* mapSize = getConsoleSize();
//    Level* level = new Level(mapSize, (char*)malloc(mapSize->getX() * mapSize->getY() * sizeof(char)));
//
//    LevelGenerParams* lvlGenParams = new LevelGenerParams(4, 15, 40);
//    LevelGenerator* generator = new LevelGenerator();
//    generator->startGeneration(level, lvlGenParams);
//    int loadMode;
//    /*generateLevel(level);
//    int (*loadLevel)(Level*);
//    
//     printf("0 from memory\n1 generate\n");
//    scanf("%d", &loadMode);
//    if (loadMode)
//         loadLevel = generateLevel;
//     else
//         loadLevel = loadMapFromFile;
//
//
//    loadLevel(level);
//    */
//    drawMap(level->getMap(), { 1, 1 }, 0, *level->getMapSize());
//    // отрисовка в консоли
//    scanf_s("%d", &loadMode); // пауза консоли
//}
/*
10
20
40
*/

