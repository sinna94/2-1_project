#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>

int game_arr[28][90];

void arr_init(int [28][90]);
void arr_print(char [28][90]);
void menu(void);
void gotoxy(int , int );
int select_num(void);
void cursor_control(char, int *, int *, int *);
void game_play();
void player_move(int [28][90], int *, int *);
void fire(int *, int *);

int main()
{
	
	menu();
	switch (select_num()) {
	case 1:
		arr_init(game_arr);
		game_play();
		break;

	default:
		break;

	}
}

void arr_init(char game_arr[28][90])
{
	int i, j;
	for (i = 0; i < 28; i++)
	{
		for (j = 0; j < 90; j++)
		{
			if (i < 4 && (j > 10 && j < 80))
				game_arr[i][j] = 1;				/*enemy*/
			else if (i == 28 && j == 45)
				game_arr[i][j] = 2;				/*player*/
			else
				game_arr[i][j] = 0;
		}
	}
}

void arr_print(char game_arr[28][90])
{
	int i, j;
	for (i = 0; i < 28; i++)
	{
		for (j = 0; j < 90; j++)
		{
			switch (game_arr[i][j]) {
			case 0:
				break;
			case 1:
				printf("@");	/*enemy*/
				break;
			case 2:
				printf("*^*");	/*player*/
				break;
			case 3:
				printf("!");	/*missile*/
				break;
			default:
				break;
			}
		}
		printf("\n");
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
	int x = 45, y = 28;

	system("cls");
	
	while (1)
	{
		gotoxy(0, 0);
		arr_print(game_arr);
		player_move(game_arr, &x, &y);
	}

}

void player_move(int game_arr[28][90], int *i, int *j)
{
	char cursor;
	int x, y, space = 0;

	while (1)
	{
		game_arr[*i][*j] = 0;
		cursor = getch();
		cursor_control(cursor, *i, *j, &space);

		j = 28;

		game_arr[*i][*j] = 2;

		if (space == 1)
		{
			x = *i;
			y = *j;
			fire(&x, &y);
		}
	}
}

void fire(int *x, int *y)
{
	x += 1;
	y -= 1;
	
	game_arr[*x][*y] = 3;
}