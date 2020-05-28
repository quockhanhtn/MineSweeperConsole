#define _CRT_SERCURE_NO_WARINGS
#define Color_Back			0
#define Color_DarkBlue		1
#define Color_DarkGreen		2
#define Color_DarkCyan		3
#define Color_DarkRed		4
#define Color_DarkPink		5
#define Color_DarkYellow	6
#define Color_DarkWhite		7
#define Color_Grey			8
#define Color_Blue			9
#define Color_Green			10
#define Color_Cyan			11
#define Color_Red			12
#define Color_Pink			13
#define ColorC_Yellow		14
#define Color_White			15

#define Color_default		7

#define key_Up		1072
#define key_Down	1080
#define key_Left	1075
#define key_Right	1077
#define key_none	-1
#define key_ESC		27
#define key_Enter	13

#define X_MIN	2
#define X_MAX	46
#define Y_MIN	2
#define Y_MAX	25

#include <windows.h>
#include <stdio.h>

struct _Point { int x, y; };

void SetVietNameseInputOutput();
void Clrscr();
void ResizeConsole(int width, int height);
void GotoXY(int column, int line);
void GotoPoint(_Point point);
int WhereX();
int WhereY();
void ShowCursor();
void HideCursor();
void PutCharColor(char ch, int color);
void TextColor(int color);
int InputKey();