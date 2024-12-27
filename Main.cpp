#include <iostream>
#include <stdio.h>
#include <windows.h>
#include "Point2D.h"
#include "Level.h"
#include "LevelGenerParams.h"
#include "LevelGenerator.h"


// отрисовка карты
void drawMap(const char* map, Point2D mapSize)
{
    int isDrawCharacter = 0;

    for (int y = 0; y < mapSize.getY(); y++)
    {
        for (int x = 0; x < mapSize.getX(); x++)
        {
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
                        putchar('?'); putchar('?');
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

int main()
{
    LevelGenerParams* lvlGenParams = new LevelGenerParams(3, 10, 20);
    Point2D* mapSize = /*getConsoleSize()*/ new Point2D(50, 50);
    char* map = (char*)malloc(mapSize->getX() * mapSize->getY() * sizeof(char));
    Level* level = new Level(mapSize, map);

    LevelGenerator* generator = new LevelGenerator();
    generator->startGeneration(level, lvlGenParams);

    
    drawMap(level->getMap(), *level->getMapSize());
    // отрисовка в консоли  
}