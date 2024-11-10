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
typedef struct Level {
    Point2D size, start, end;

    //Point2D* map;
    // 2d array [size.x][size.y]
};

// параметры генерации уровня
typedef struct LevelGenerParams {
    int
        minRoomSize = 5,
        maxRoomSize = 20,
        probabOfDivide = 40;
    // вероятность деления комнаты [0..100]
};


/*
* start = левый нижний угол [0][0]
* end = правый верхний угол [x][y]
*/
typedef struct Room  {
    int id;
    Point2D startPoint, endPoint;
    int isdivideComplete = 0;
    // булевая переменная - будет ли комната делиться ещё
};

int randomInt(int _min, int _max)
{
    /*std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(_min, _max);
    return dist6(rng);*/
    //srand(time(0));
    int r = rand() % (_max - _min + 1) + _min;
    printf("/// random = %d /// [%d, %d]\n", r, _min, _max);
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

void roomArrayToMap(Room* rooms, int n, char* map, Point2D mapSize)
{
    // заполняем все стенами
    for (int y = 0; y < mapSize.y; y++)
    {
        for (int x = 0; x < mapSize.x; x++)
        {
            *(map + y * mapSize.x + x) = WALL_TAG;
        }
    }

    // заполняем выборочно пустым пространством (комнаты)
    for (int i = 0; i < n; i++)
    {
        for (int y = rooms[i].startPoint.y; y <= rooms[i].endPoint.y; y++)
        {
            for (int x = rooms[i].startPoint.x; x <= rooms[i].endPoint.x; x++)
            {
                *(map + y * mapSize.x + x) = EMPTY_TAG;
            }
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

   /* char* map = (char*)malloc(60 * 30 * sizeof(char));
    roomArrayToMap(_rooms, new_n, map, { 60, 30 });
    printf("\n\n");
    drawMap(map, { 1, 1 }, 0, { 60, 30 });*/

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

int main()
{
    srand(time(0));
    for (int i = 0; i < 20; i++)
    printf("%d\n", rand() % (100 - 5 + 1) + 5);

    Point2D* mapSize = getConsoleSize();

    Level level;
    level.size = *mapSize;
    
    LevelGenerParams lvlGenParams;

    Room* rooms = (Room*) malloc(sizeof(Room));
    int n = 1;
    rooms[0] = {
        rand(),
        {1, 1},
        {level.size.x - 2, level.size.y - 2}
    };
    rooms = divideRooms(&rooms[0], &n, &lvlGenParams);



    // ------------------------
    char* map = (char*)malloc(level.size.x * level.size.y * sizeof(char));
    roomArrayToMap(rooms, n, map, level.size);
    printf("\n\n");
    drawMap(map, { 1, 1 }, 0, level.size);


    scanf_s("%d", &n); // пауза консоли
}