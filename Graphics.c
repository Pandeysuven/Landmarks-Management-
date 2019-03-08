/*
	//Graphic.c
	//Contains graphics functions such as moving cursor to coordinates, creating table
*/
#include "Functions.h"
#include <stdio.h>

#define COLOR_CYAN 9

void GoToXY(int column, int line)
{
    COORD coord;
    coord.X = column;
    coord.Y = line;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, coord);
}

COORD GoToMiddle(int correctionX, int correctionY)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD middle_position = { csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };
    GoToXY(middle_position.X / 2 + correctionX, middle_position.Y / 2 + correctionY);
    return middle_position;
}


void CreateTable(int length, int breadth)
{
    HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(stdHandle, COLOR_CYAN);
    const int l = length / 2, b = breadth / 2;
    GoToMiddle(-(l), -(b));
    printf("%c", 218);                          //upper left

    GoToMiddle(l, -(b));
    printf("%c", 191);                          //upper right

    GoToMiddle(-l, b);
    printf("%c", 192);

    GoToMiddle(l, b);
    printf("%c", 217);

    for(int j = l - 1; j >= -(l - 1); j--)      //top
    {
        GoToMiddle(j, -b);
        printf("%c", 196);
        GoToMiddle(j, b);                     //bottom
        printf("%c", 196);
    }

    for(int j = b - 1; j >= -(b - 1); j--)
    {
        GoToMiddle(-l, j);					   //left
        printf("%c", 179);
        GoToMiddle(l, j);
        printf("%c", 179);
    }

    SetConsoleTextAttribute(stdHandle, COLOR_NORMAL);
}
