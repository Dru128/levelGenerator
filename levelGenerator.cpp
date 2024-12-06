// levelGenerator.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <random>

#define EMPTY_TAG '1'
#define WALL_TAG '2'
#define START_TAG '4'
#define FINISH_TAG '5'
#define ENEMY_TAG '6'

typedef struct Point2D {
    int 
        x = 0,
        y = 0;
};

// данные уровня
typedef struct Level {
    Point2D size, start, end;
    char* map;
    // 2d array [size.x][size.y]
};

// параметры генерации уровня
typedef struct LevelGenerParams {
    int
        minRoomSize = 10,
        maxRoomSize = 20,
        probabOfDivide = 40;
    // вероятность деления комнаты [0..100]
};


/*
* start = левый нижний угол 
* end = правый верхний угол 
*/
typedef struct Room  {
    int id;
    Point2D startPoint, endPoint;
    int isdivideComplete = 0;
    // булевая переменная - будет ли комната делиться ещё
};


typedef struct RoomConnected {
    int roomIndex;
    // индекс комнаты в исходном массиве

    Point2D wallStart, wallEnd;
    // начало и конец границы комнат
    int weight; // вес ребра
    int isConnectComplete = 0;
};
typedef struct RoomConnection {
    Room *curRoom;
    // текущая комната
    RoomConnected* connectedRooms; // выделить память !!!
    int ConnectedCount = 0;
    // её соседи (массив)
};

int randomInt(int _min, int _max)
{
    int r = rand() % (_max - _min + 1) + _min;
    return r;
}

// -----------------< graphic >----------------------
// отрисовка карты
void drawMap(const char* map, Point2D player, int countEnemies, Point2D mapSize)
{
    //system("CLS"); // выдает ошибку
    //clearScreen();
    int isDrawCharacter = 0;

    for (int y = 0; y < mapSize.y; y++)
    {
        for (int x = 0; x < mapSize.x; x++)
        {
            //if (x >= WIDTH || x >= WIDTH) return;
            isDrawCharacter = 0;
            if (player.x == x && player.y == y)
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
                switch (*(map + y * mapSize.x + x))
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
    Point2D size;

    HANDLE hWndConsole;
    if (hWndConsole = GetStdHandle(-12))
    {
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
        {
            size.x = (consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1) / 2;
            size.y = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
            return &size;
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

    system("mode con cols=120 lines=30");
    size.x = 60;
    size.y = 30;
    return &size;
}

void roomArrayToMap(RoomConnection* roomsGraph, int n, char* map, Point2D mapSize)
{
    // заполняем все стенами
    for (int y = 0; y < mapSize.y; y++)
    {
        for (int x = 0; x < mapSize.x; x++)
        {
            *(map + y * mapSize.x + x) = WALL_TAG;
        }
    }

    int count = 0,
        * connectedDraw = (int*)calloc(n*n, sizeof(int));
    // заполняем выборочно пустым пространством (комнаты)
    for (int i = 0; i < n; i++)
    {
        for (int y = roomsGraph[i].curRoom->startPoint.y; y <= roomsGraph[i].curRoom->endPoint.y; y++)
        {
            for (int x = roomsGraph[i].curRoom->startPoint.x; x <= roomsGraph[i].curRoom->endPoint.x; x++)
            {
                *(map + y * mapSize.x + x) = EMPTY_TAG;
            }
        }
        // TODO
        for (int j = 0; j < roomsGraph[i].ConnectedCount && roomsGraph[i].connectedRooms[j].isConnectComplete; j++)
        {
            // исправить два прохода на один!
            int randX = randomInt(roomsGraph[i].connectedRooms[j].wallStart.x, roomsGraph[i].connectedRooms[j].wallEnd.x),
                randY = randomInt(roomsGraph[i].connectedRooms[j].wallStart.y, roomsGraph[i].connectedRooms[j].wallEnd.y);
            *(map + randY * mapSize.x + randX) = EMPTY_TAG;
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

Room* divideRooms(Room* rooms, int *n, const LevelGenerParams* lvlGenParams)
{
    Room* _rooms = (Room*)malloc(2 * (*n) * sizeof(Room));
    int new_n = 0;

    for (int i = 0; i < (*n); i++)
    {
        printf("i = %d) [%d][%d] | [%d][%d]\n", i,
            rooms[i].startPoint.x, rooms[i].startPoint.y,
            rooms[i].endPoint.x, rooms[i].endPoint.y);


        int
            delta_x = rooms[i].endPoint.x - rooms[i].startPoint.x,
            delta_y = rooms[i].endPoint.y - rooms[i].startPoint.y,
            delta_max = max(delta_x, delta_y);
        
        rooms[i].isdivideComplete = 
            lvlGenParams->maxRoomSize > delta_max &&
            (
             rooms[i].isdivideComplete ||
             lvlGenParams->probabOfDivide < randomInt(0, 100) ||
             delta_max < 2 * lvlGenParams->minRoomSize
            );
        // Здесь мы решаем будем делить комнату или просто скопируем в новый массив
        // И пометим её как неделимую 

        if (rooms[i].isdivideComplete == 0)
        {
            if (delta_x > delta_y)
            {
                // место разреза комнаты
                int divide = randomInt(
                    rooms[i].startPoint.x + lvlGenParams->minRoomSize,
                    rooms[i].endPoint.x - lvlGenParams->minRoomSize
                );

                // поделили на 2 комнаты по оси X
                _rooms[new_n++] = {
                    rand(),
                    { rooms[i].startPoint.x, rooms[i].startPoint.y },
                    { divide - 1, rooms[i].endPoint.y },
                    rooms[i].isdivideComplete
                };
                _rooms[new_n++] = {
                    rand(),
                    { divide + 1, rooms[i].startPoint.y },
                    { rooms[i].endPoint.x, rooms[i].endPoint.y },
                    rooms[i].isdivideComplete
                };
            }
            else
            {
                // место разреза комнаты
                int divide = randomInt(
                    rooms[i].startPoint.y + lvlGenParams->minRoomSize,
                    rooms[i].endPoint.y - lvlGenParams->minRoomSize
                );

                // поделили на 2 комнаты по оси Y
                _rooms[new_n++] = {
                    rand(),
                    { rooms[i].startPoint.x, rooms[i].startPoint.y },
                    { rooms[i].endPoint.x, divide - 1 },
                    rooms[i].isdivideComplete
                };
                _rooms[new_n++] = {
                    rand(),
                    { rooms[i].startPoint.x, divide + 1 },
                    { rooms[i].endPoint.x, rooms[i].endPoint.y },
                    rooms[i].isdivideComplete
                };
            }
        }
        else 
        {
            // комнату поделить нельзя 
            // копируем ее в новый массив комнат
            _rooms[new_n++] = {
                rand(),
                { rooms[i].startPoint.x, rooms[i].startPoint.y },
                { rooms[i].endPoint.x, rooms[i].endPoint.y },
                rooms[i].isdivideComplete
            };
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
        
        *commonStart = max(start1, start2);
        *commonEnd = min(end1, end2);
        // общий отрезок

        return 1;
    }
    else
        return 0; // отрезки не пересекаются
}

void makeRoomGraph(Room* rooms, int n, RoomConnection* roomsGraph)
{ 
    for (int i = 0; i < n; i++)
    {
        roomsGraph[i].curRoom = &rooms[i];
        roomsGraph[i].connectedRooms = (RoomConnected*)malloc(n * sizeof(RoomConnected));
        roomsGraph[i].ConnectedCount = 0;
    }

    for (int cur_i = 0; cur_i < n; cur_i++)
    {
        for (int i = cur_i; i < n; i++)
        {
            if (cur_i == i) continue;

            if (rooms[cur_i].endPoint.x + 2 == rooms[i].startPoint.x)
            { // проверка правого края
                int startY, endY;
                if (commonInterval(
                    rooms[cur_i].startPoint.y, rooms[cur_i].endPoint.y,
                    rooms[i].startPoint.y, rooms[i].endPoint.y,
                    &startY, &endY))
                {
                    int weight = randomInt(0, 100);
                    roomsGraph[cur_i].connectedRooms[roomsGraph[cur_i].ConnectedCount++] = {
                        i,
                        { rooms[cur_i].endPoint.x + 1, startY },
                        { rooms[cur_i].endPoint.x + 1, endY },
                        weight
                    };
                    roomsGraph[i].connectedRooms[roomsGraph[i].ConnectedCount++] = {
                        cur_i,
                        { rooms[i].startPoint.x - 1, startY },
                        { rooms[i].startPoint.x - 1, endY },
                        weight
                    };
                }
            } else if (rooms[cur_i].startPoint.x - 2 == rooms[i].endPoint.x)
            { // проверка левого края
                int startY, endY;
                if (commonInterval(
                    rooms[cur_i].startPoint.y, rooms[cur_i].endPoint.y,
                    rooms[i].startPoint.y, rooms[i].endPoint.y,
                    &startY, &endY))
                {
                    int weight = randomInt(0, 100);
                    roomsGraph[cur_i].connectedRooms[roomsGraph[cur_i].ConnectedCount++] = {
                        i,
                        { rooms[cur_i].startPoint.x - 1, startY },
                        { rooms[cur_i].startPoint.x - 1, endY },
                        weight
                    };
                    roomsGraph[i].connectedRooms[roomsGraph[i].ConnectedCount++] = {
                        cur_i,
                        { rooms[i].endPoint.x + 1, startY },
                        { rooms[i].endPoint.x + 1, endY },
                        weight
                    };
                }
            } else if (rooms[cur_i].endPoint.y + 2 == rooms[i].startPoint.y)
            { // проверка верхнего края
                int startX, endX;
                if (commonInterval(
                    rooms[cur_i].startPoint.x, rooms[cur_i].endPoint.x,
                    rooms[i].startPoint.x, rooms[i].endPoint.x,
                    &startX, &endX))
                {
                    int weight = randomInt(0, 100);
                    roomsGraph[cur_i].connectedRooms[roomsGraph[cur_i].ConnectedCount++] = {
                        i,
                        { startX, rooms[cur_i].endPoint.y + 1 },
                        { endX, rooms[cur_i].endPoint.y + 1 },
                        weight
                    };
                    roomsGraph[i].connectedRooms[roomsGraph[i].ConnectedCount++] = {
                        cur_i,
                        { startX, rooms[i].startPoint.y - 1 },
                        { endX, rooms[i].startPoint.y - 1 },
                        weight
                    };
                }
            } else if (rooms[cur_i].startPoint.y - 2 == rooms[i].endPoint.y)
            { // проверка нижнего края
                int startX, endX;
                if (commonInterval(
                    rooms[cur_i].startPoint.x, rooms[cur_i].endPoint.x,
                    rooms[i].startPoint.x, rooms[i].endPoint.x,
                    &startX, &endX))
                {
                    int weight = randomInt(0, 100);
                    roomsGraph[cur_i].connectedRooms[roomsGraph[cur_i].ConnectedCount++] = {
                        i,
                        { startX, rooms[cur_i].startPoint.y - 1 },
                        { endX, rooms[cur_i].startPoint.y - 1 },
                        weight
                    };
                    roomsGraph[i].connectedRooms[roomsGraph[i].ConnectedCount++] = {
                        cur_i,
                        { startX, rooms[i].endPoint.y + 1 },
                        { endX, rooms[i].endPoint.y + 1 },
                        weight
                    };
                }
            }
        
        }
    }
}

void makeRoomConnections(int n, RoomConnection* roomsGraph)
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

            int conCount = roomsGraph[leaves[il]].ConnectedCount;
            for (int i = 0; i < conCount; i++)
            { // перебираем соседей листов
                if (roomsGraph[leaves[il]].connectedRooms[i].isConnectComplete == 0)
                { // соединения еще нет (1)
                    int flag = 1;
                    for (int _il = 0; _il < nLeaves && flag; _il++)
                        flag = leaves[_il] != roomsGraph[leaves[il]].connectedRooms[i].roomIndex;
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

                    printf("leaf = %d, second = %d, weight = %d \n", leaves[il], roomsGraph[leaves[il]].connectedRooms[i].roomIndex,roomsGraph[leaves[il]].connectedRooms[i].weight);
                    if ((isInitMinWeight == 0 || // инициализируем первым значением
                        roomsGraph[leaves[il]].connectedRooms[i].weight < minWeightRoom->weight) && flag)
                    {
                        isInitMinWeight = 1;
                        minWeightRoom = &roomsGraph[leaves[il]].connectedRooms[i];
                        targetLeaveIndex = il;
                    }

                }
            }
        }

        // мы выбрали вершину графа (комнату), 
        // которая граничит с одним из листов, 
        // не является листом и обладает минимальным весом
        printf("nleaf = %d, isInit = %d, targetI = %d, roomIndex = %d\n", nLeaves, isInitMinWeight, leaves[targetLeaveIndex], minWeightRoom->roomIndex);
        if (isInitMinWeight == 1)
        {
            leaves[nLeaves] = minWeightRoom->roomIndex;
            nLeaves++;


            for (int k = 0; k < roomsGraph[leaves[targetLeaveIndex]].ConnectedCount; k++)
                if (roomsGraph[leaves[targetLeaveIndex]].connectedRooms[k].roomIndex == minWeightRoom->roomIndex)
                {
                    roomsGraph[leaves[targetLeaveIndex]].connectedRooms[k].isConnectComplete = 1;
                    printf("i = %d", k);

                    break;
                }

            for (int k = 0; k < roomsGraph[minWeightRoom->roomIndex].ConnectedCount; k++)
                if (roomsGraph[minWeightRoom->roomIndex].connectedRooms[k].roomIndex == leaves[targetLeaveIndex])
                {
                    roomsGraph[minWeightRoom->roomIndex].connectedRooms[k].isConnectComplete = 1;
                    printf(" j = %d\n", k);

                    break;
                }
        }
    }
}


int main()
{
    srand(time(0));
    // инициализация генератора случайных чисел

    Point2D* mapSize = getConsoleSize();

    Level level;
    level.size = *mapSize;
    // размер карты
    level.map = (char*)malloc(level.size.x * level.size.y * sizeof(char));
    // дискретное представление уровня, где каждой координате
    // соответсвует препятствие или пустое пространство

    LevelGenerParams lvlGenParams;

    Room* rooms = (Room*) malloc(sizeof(Room));
    // массив комнат
    int n = 1;
    rooms[0] = {
        rand(),
        {1, 1},
        {level.size.x - 2, level.size.y - 2}
    };
    rooms = divideRooms(rooms, &n, &lvlGenParams);
    // делим на комнаты

    RoomConnection* roomsGraph = (RoomConnection*)malloc(n * sizeof(RoomConnection));;
    makeRoomGraph(rooms, n, roomsGraph);
    // построение графа комнат-соседей (список смежности)
    // каждая комната харнит индексы в исходном массиве своих комнат-соседей

    /*for (int i = 0; i < n; i++)
    {
        printf(">>>%d\n", i);
        for (int j = 0; j < roomsGraph[i].ConnectedCount; j++)
            printf("id = %d (%d, %d) : (%d, %d) --- isConnect %d | weight %d\n", roomsGraph[i].connectedRooms[j].roomIndex,
                roomsGraph[i].connectedRooms[j].wallStart.x,
                roomsGraph[i].connectedRooms[j].wallStart.y,
                roomsGraph[i].connectedRooms[j].wallEnd.x,
                roomsGraph[i].connectedRooms[j].wallEnd.x,
                roomsGraph[i].connectedRooms[j].isConnectComplete,
                roomsGraph[i].connectedRooms[j].weight
                );
    }*/
    makeRoomConnections(n, roomsGraph);
    // Алгоритм Прима - строим минимальное остовное дерево взвешенного связного неориентированного графа,
    // где веса ребер - веса связей комнат RoomConnected.weight
    roomArrayToMap(roomsGraph, n, level.map, level.size);
    // инициализируем level.map
    printf("\n\n");
    drawMap(level.map, { 1, 1 }, 0, level.size);
    // отрисовка в консоли, вместо этого может быть другая функция
    // или создание карты на движке, например на Unity

    scanf_s("%d", &n); // пауза консоли
}