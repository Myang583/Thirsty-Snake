#pragma once
#include <windows.h>
#define MOVE_LEFT   1
#define MOVE_RIGHT  2
#define MOVE_UP     3
#define MOVE_DOWN   4
#define HOLD_ON     5
void initMainFrame(HINSTANCE, HWND, HDC);
void drawMainFrame(HINSTANCE, HWND, HDC);
bool moveSnake(int, int);
void checkCollision();
DWORD WINAPI timeCaculator(LPVOID);
int getCurrentTime();
void restartGame(HDC);
void setRestart(bool);
void setDirection(int);
int getDirection();