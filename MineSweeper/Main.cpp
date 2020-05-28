#define _CRT_SECURE_NO_WARNINGS
#include "Graphic.h"
#include <cstdio>
#include <Windows.h>
#include <ctime>
#include <conio.h>

int OpenArr[100][100] = { 0 };
int MineArr[100][100] = { 0 };
int FlagArr[100][100] = { 0 };

int level, height, width, numMine, numFlag;
int firstSelect = 1;

void NewGame();
void SetLevel();
void GenMine(int x, int y);
void ShowGame();
void OpenCell(int i, int j);
void OpenAll();
void DrawLevel(int lv);
void CustomLevel();
int IsWin();

int main()
{
	srand(time(NULL));
	//SetVietNameseInputOutput();

	printf("Huong dan:\n");
	printf("\tNhan phim'A','W','D','S' hoac cac phim mui ten de di chuyen\n");
	printf("\tNhan phim 'F' de cam co\n");
	printf("\tNhan phim 'Enter' de mo 1 o\n");

	wprintf(L"Mo tat ca cac o khong co min de gianh chien thang");
	//_getch();
	
	NewGame();
	while (true)
	{
		
		//ShowGame();
		char key = _getch();
		int i = WhereY() - 2 + 1;
		int j = WhereX() / 2 - 2 + 1;
		switch (key)
		{
		case 'A':
		case 'a':
		case key_Left - 1000:
			if (WhereX() > 4) {
				GotoXY(WhereX() - 2, WhereY());
			}
			break;

		case 'D':
		case 'd':
		case key_Right - 1000:
			if (WhereX() < width * 2 + 2) {
				GotoXY(WhereX() + 2, WhereY());
			}
			break;

		case 'W':
		case 'w':
		case key_Up - 1000:
			if (WhereY() > 2) {
				GotoXY(WhereX(), WhereY() - 1);
			}
			break;

		case 'S':
		case 's':
		case key_Down - 1000:
			if (WhereY() < height + 1) {
				GotoXY(WhereX(), WhereY() + 1);
			}
			break;

		case 'f':
		case 'F':
			
			if (OpenArr[i][j] == 1) { break; }
			
			if (FlagArr[i][j] == 0) {
				if (numFlag <= 0) { break; }
				FlagArr[i][j] = 1;
				numFlag--;

			}
			else {
				FlagArr[i][j] = 0;
				numFlag++;
			}
			ShowGame();
			break;

		case key_Enter:
			if (firstSelect) { 
				GenMine(i, j);
				firstSelect = 0;
			}
			if (FlagArr[i][j] == 1) {
				break;
			}

			if (MineArr[i][j] == -1) {
				
				OpenAll();
				ShowGame();
				GotoXY(2, height + 4);
				TextColor(Color_Red);
				printf("Game Over");
				TextColor(Color_default);
				GotoXY(2, height + 5);
				printf("Press any key to replay");
				GotoXY(2, height + 6);
				printf("Press ESC key to exit");

				char _key = _getch();
				if (_key == key_ESC) { exit(0); }
				else { 
					NewGame();
					break;
				}
			}

			OpenCell(i, j);
			ShowGame();
		}
		if (IsWin()) {
			OpenAll();
			ShowGame();
			GotoXY(2, height + 5);
			TextColor(Color_Red);
			printf("You win");
			TextColor(Color_default);
			GotoXY(2, height + 6);
			printf("Press any key to replay");
			GotoXY(2, height + 7);
			printf("Press ESC key to exit");

			char _key = _getch();
			if (_key == key_ESC) { exit(0); }
			else {
				NewGame();
				break;
			}
		}
	}
	return 0;
}

void SetLevel()
{
	ResizeConsole(1200, 500);
	system("cls");
	int x = 1, y = 2;
	GotoXY(x, y++);
	printf(" ______ ______       ______ ______ ______            ______ _    _ ______      "); GotoXY(x, y++);
	printf(" |_____ |_____ |     |_____ |        |         |     |_____  \\  /  |_____ |    "); GotoXY(x, y++);
	printf(" _____| |_____ |____ |_____ |____    |         |____ |_____   \\/   |_____ |____"); GotoXY(x, y++);
	DrawLevel(1);
	level = 1;
	HideCursor();
	while (true) {
		char key = _getch();
		switch (key)
		{
		case 'A':
		case 'a':
		case key_Left - 1000:
			if (level > 1) {
				level--;
			}
			else { level = 4; }
			break;
		case 'D':
		case 'd':
		case key_Right - 1000:
			if (level < 4) {
				level++;
			}
			else { level = 1; }
			break;
		case key_Enter:
			switch (level)
			{
			case 1:
				height = 8;
				width = 10;
				numMine = 10;
				ResizeConsole(56 + width * 30 + 56 + 20, 60 + height * 28 + 260);
				//ResizeConsole(440, 550);
				break;
			case 2:
				height = 14;
				width = 18;
				numMine = 40;
				//ResizeConsole(668, 740);
				ResizeConsole(56 + width * 30 + 56 + 20, 60 + height * 28 + 260);
				break;
			case 3:
				height = 20;
				width = 24;
				numMine = 99;
				//ResizeConsole(845, 900);
				ResizeConsole(56 + width * 30 + 56 + 20, 60 + height * 28 + 260);
				break;
			case 4:
				CustomLevel();
				ResizeConsole(56 + width * 30 + 56 + 20, 60 + height * 28 + 260);
				break;
			}
			numFlag = numMine;
			ShowCursor();
			return;
		}
		DrawLevel(level);
	}
}

void NewGame()
{
	SetLevel();
	GotoXY(4, 2);
	firstSelect = 1;
	for (int i = 0; i < height + 2; i++) {
		for (int j = 0; j < width + 2; j++) {
			MineArr[i][j] = 0;
			OpenArr[i][j] = 0;
			FlagArr[i][j] = 0;
		}
	}
	ShowGame();
}

void GenMine(int x, int y)
{
	int i, j, k = 0;
	while (true) {
		i = rand() % (height) + 1;
		j = rand() % (width) + 1;
		if (MineArr[i][j] == 0 && i != y && j != x) {
			MineArr[i][j] = -1;
			k++;
		}
		if (k == numMine) { break; }
	}
	int dx[] = { -1,-1,-1,0,0,1,1,1 };
	int dy[] = { -1,0,1,-1,1,-1,0,1 };

	for (i = 1; i < height + 1; i++) {
		for (j = 1; j < width + 1; j++) {
			if (MineArr[i][j] != -1) {
				for (k = 0; k < 8; k++) {
					if (MineArr[i + dx[k]][j + dy[k]] == -1) {
						MineArr[i][j]++;
					}
				}
			}
		}
	}

	FILE* fo;
	fo = fopen("result.txt", "wt");
	for (i = 1; i < height + 1; i++) {
		for (j = 1; j < width + 1; j++) {
			fprintf(fo, "%d\t", MineArr[i][j]);
		}
		fprintf(fo, "\n");
	}
	fclose(fo);
}

void ShowGame()
{
	int X = WhereX();
	int Y = WhereY();
	Clrscr();
	int x = 2, y = 1;
	int i;
	GotoXY(x, y++);
	putchar(201);
	for (i = 0; i < width * 2 + 2; i++) { putchar(205); }
	putchar(187);
	
	for (i = 1; i < height + 1; i++) {
		GotoXY(x, y++);
		printf("%c ", 186);
		for (int j = 1; j < width + 1; j++) {
			if (FlagArr[i][j] == 1) {
				PutCharColor('x', 4);
			}
			else {
				if (OpenArr[i][j] == 0) {
					if ((i + j) % 2 == 0) {
						PutCharColor(32, 160);
					}
					else { PutCharColor(32, 47); }
				}
				else {
					switch (MineArr[i][j])
					{
					case -1:
						PutCharColor('*', 192);
						break;
					case 0:
						if ((i + j) % 2 == 0) {
							PutCharColor(32, 224);
						}
						else { PutCharColor(32, 96); }
						break;
					case 1:
						PutCharColor('1', 1);
						break;
					case 2:
						PutCharColor('2', 2);
						break;
					case 3:
						PutCharColor('3', 3);
						break;
					case 4:
						PutCharColor('4', 4);
						break;
					case 5:
						PutCharColor('5', 5);
						break;
					case 6:
						PutCharColor('6', 6);
						break;
					case 7:
						PutCharColor('7', 7);
						break;
					case 8:
						PutCharColor('8', 8);
						break;
					}
				}
			}
		}
		printf(" %c", 186);
	}

	GotoXY(x, y++);
	putchar(200);
	for (i = 0; i < width * 2 + 2; i++) { putchar(205); }
	putchar(188);
	GotoXY(x, y);
	printf("Level: ");
	switch (level)
	{
	case 1:
		printf("Easy", height, width);
		break;
	case 2:
		printf("Medium (%dx%d)", height, width);
		break;
	case 3:
		printf("Hard (%dx%d)", height, width);
		break;
	case 4:
		printf("Custom (%dx%d)", height, width);
		break;
	}
	if (width * 2 + 2 > 30) {
		if (numFlag < 10) {
			GotoXY(width * 2 - 1, y);
			printf("Flag: %d", numFlag);
		}
		else if (numFlag < 100) {
			GotoXY(width * 2 - 2, y);
			printf("Flag: %d", numFlag);
		}
		else {
			GotoXY(width * 2 - 3, y);
			printf("Flag: %d", numFlag);
		}
	}
	else {
		GotoXY(x, y + 1);
		printf("Flag: %d", numFlag);
	}

	GotoXY(X, Y);
}

void OpenAll()
{
	for (int i = 1; i < height + 1; i++) {
		for (int j = 1; j < width + 1; j++) {
			OpenArr[i][j] = 1;
			FlagArr[i][j] = 0;
		}
	}
}

void OpenCell(int i, int j) 
{
	if (MineArr[i][j] == -1) { return; }
	OpenArr[i][j] = 1;
	if (MineArr[i][j] > 0) { return; }

	int dx[] = { -1,-1,-1,0,0,1,1,1 };
	int dy[] = { -1,0,1,-1,1,-1,0,1 };
	for (int k = 0; k < 8; k++) {
		if (i + dx[k] > 0 && i + dx[k] < height + 2 && j + dy[k] > 0 && j + dy[k] < width + 2) {
			if (OpenArr[i + dx[k]][j + dy[k]] == 0) {
				OpenCell(i + dx[k], j + dy[k]);
			}
		}
	}
}

int IsWin()
{
	int cellOpen = 0;
	for (int i = 1; i < height + 1; i++) {
		for (int j = 1; j < width + 1; j++) {
			if (OpenArr[i][j] == 1) {
				cellOpen++;
			}
		}
	}
	if (cellOpen == height * width - numMine)
	{
		return 1;
	}
	return 0;
}

void DrawLevel(int lv)
{
	int x = 14;
	int frame[2][7] = { {218,196,191,179,192,196,217},{201,205,187,186,200,205,188} };
	int select = 0;

	GotoXY(x, 6);
	putchar(frame[select][0]);
	for (int i = 0; i < 10; i++) { putchar(frame[select][1]); }
	putchar(frame[select][2]);
	GotoXY(x, 7);
	printf("%c   Easy   %c", frame[select][3], frame[select][3]);
	GotoXY(x, 8);
	putchar(frame[select][4]);
	for (int i = 0; i < 10; i++) { putchar(frame[select][5]); }
	putchar(frame[select][6]);

	GotoXY(x + 14, 6);
	putchar(frame[select][0]);
	for (int i = 0; i < 10; i++) { putchar(frame[select][1]); }
	putchar(frame[select][2]);
	GotoXY(x + 14, 7);
	printf("%c  Medium  %c", frame[select][3], frame[select][3]);
	GotoXY(x + 14, 8);
	putchar(frame[select][4]);
	for (int i = 0; i < 10; i++) { putchar(frame[select][5]); }
	putchar(frame[select][6]);

	GotoXY(x + 28, 6);
	putchar(frame[select][0]);
	for (int i = 0; i < 10; i++) { putchar(frame[select][1]); }
	putchar(frame[select][2]);
	GotoXY(x + 28, 7);
	printf("%c   Hard   %c", frame[select][3], frame[select][3]);
	GotoXY(x + 28, 8);
	putchar(frame[select][4]);
	for (int i = 0; i < 10; i++) { putchar(frame[select][5]); }
	putchar(frame[select][6]);

	GotoXY(x + 42, 6);
	putchar(frame[select][0]);
	for (int i = 0; i < 10; i++) { putchar(frame[select][1]); }
	putchar(frame[select][2]);
	GotoXY(x + 42, 7);
	printf("%c  Custom  %c", frame[select][3], frame[select][3]);
	GotoXY(x + 42, 8);
	putchar(frame[select][4]);
	for (int i = 0; i < 10; i++) { putchar(frame[select][5]); }
	putchar(frame[select][6]);

	TextColor(Color_Red);
	select = 1;
	switch (lv)
	{
	case 1:
		GotoXY(x, 6);
		putchar(frame[select][0]);
		for (int i = 0; i < 10; i++) { putchar(frame[select][1]); }
		putchar(frame[select][2]);
		GotoXY(x, 7);
		printf("%c   Easy   %c", frame[select][3], frame[select][3]);
		GotoXY(x, 8);
		putchar(frame[select][4]);
		for (int i = 0; i < 10; i++) { putchar(frame[select][5]); }
		putchar(frame[select][6]);
		break;
	case 2:
		GotoXY(x + 14, 6);
		putchar(frame[select][0]);
		for (int i = 0; i < 10; i++) { putchar(frame[select][1]); }
		putchar(frame[select][2]);
		GotoXY(x + 14, 7);
		printf("%c  Medium  %c", frame[select][3], frame[select][3]);
		GotoXY(x + 14, 8);
		putchar(frame[select][4]);
		for (int i = 0; i < 10; i++) { putchar(frame[select][5]); }
		putchar(frame[select][6]);
		break;
	case 3:
		GotoXY(x + 28, 6);
		putchar(frame[select][0]);
		for (int i = 0; i < 10; i++) { putchar(frame[select][1]); }
		putchar(frame[select][2]);
		GotoXY(x + 28, 7);
		printf("%c   Hard   %c", frame[select][3], frame[select][3]);
		GotoXY(x + 28, 8);
		putchar(frame[select][4]);
		for (int i = 0; i < 10; i++) { putchar(frame[select][5]); }
		putchar(frame[select][6]);
		break;
	case 4:
		GotoXY(x + 42, 6);
		putchar(frame[select][0]);
		for (int i = 0; i < 10; i++) { putchar(frame[select][1]); }
		putchar(frame[select][2]);
		GotoXY(x + 42, 7);
		printf("%c  Custom  %c", frame[select][3], frame[select][3]);
		GotoXY(x + 42, 8);
		putchar(frame[select][4]);
		for (int i = 0; i < 10; i++) { putchar(frame[select][5]); }
		putchar(frame[select][6]);
		break;
	}
	TextColor(Color_default);
}

void CustomLevel()
{
	ResizeConsole(1200, 500);
	system("cls");
	int x = 1, y = 2;
	GotoXY(x, y++);
	printf(" ______ ______       ______ ______ ______            ______ _    _ ______      "); GotoXY(x, y++);
	printf(" |_____ |_____ |     |_____ |        |         |     |_____  \\  /  |_____ |    "); GotoXY(x, y++);
	printf(" _____| |_____ |____ |_____ |____    |         |____ |_____   \\/   |_____ |____"); GotoXY(x, y++);
	GotoXY(x, y++);
	printf("Custom level : ");
	GotoXY(x, y++);
	printf("\t- Input your height: ");
	scanf("%d", &height);
	GotoXY(x, y++);
	printf("\t- Input your width: ");
	scanf("%d", &width);
	GotoXY(x, y++);
	printf("\t- Input number mine: ");
	scanf("%d", &numMine);
}