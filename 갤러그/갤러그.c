#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

void menu(void);
void gotoxy(int , int );
int select_num(void);
void cursor_control(char, int *, int *, int *);
void game_play();
void player_move(int*, int*);
void fire(int, int);

int main()
{
	menu();
	switch (select_num()) {
	case 1:
		game_play();
		break;

	default:
		break;

	}
}

void menu()
{
	int sel_num = 0;
	gotoxy(40, 23);
	puts("1. game start");
	gotoxy(40, 24);
	puts("2. score");
	gotoxy(40, 25);
	puts("3. exit");

	
}

void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

int select_num(void)
{
	int x = 38, y = 23, space = 0;
	char cursor;

	gotoxy(x, y);
	printf("ขั");

	while(1) 
	{
		cursor = getch();
		gotoxy(x, y);
		printf(" ");
		cursor_control(cursor, &x, &y, &space);
		
		x = 38;
			
		if ((y > 25))
			y -= 1;

		else if (y < 23)
			y += 1;

		gotoxy(x, y);
		printf("ขั");

		if (space == 1)
			break;
	}

	switch (y) {
	case 23:
		return 1;
		break;

	case 24:
		return 2;
		break;

	case 25:
		exit(1);

	default:
		break;
	}
}

void cursor_control(char cursor, int *x, int *y, int *space)
{
	switch (cursor)
	{
	case 72:				//up
		if (*y > 0)
			*y -= 1;
		break;

	case 80:
		*y += 1;			//down
		break;

	case 75:				//left
		if (*x > 0)
			*x -= 1;
		break;

	case 77:				//right
		*x += 1;
		break;

	case 32:				//SP
		*space = 1;
		break;

	default:
		break;
	}
}

void game_play()
{
	int x = 20, y = 28;

	system("cls");
	while (1)
	{
		player_move(&x, &y);
	}

}

void player_move(int *x, int *y)
{
	char cursor;
	int space = 0;

	gotoxy(*x, *y);
	printf("*^*");
	while (1)
	{
		cursor = getch();
		gotoxy(*x, *y);
		printf("   ");
		cursor_control(cursor, x, y, &space);

		*y = 28;

		gotoxy(*x, *y);
		printf("*^*");

		if (space == 1)
		{
			fire(*x, *y);
			break;
		}
	}
}

void fire(int x, int y)
{
	x += 1;
	y -= 1;
	
	while (y != 0) 
	{
		gotoxy(x, y);
		printf(" ");
		gotoxy(x, --y);
		printf("!");
		Sleep(50);
	}
	gotoxy(x, y);
	printf(" ");
}