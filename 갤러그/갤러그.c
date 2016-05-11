#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define N 27
#define M 61

char game_arr[N][M];

void arr_init(char[N][M]);
int arr_print(char[N][M], int*);
void menu(void);
void gotoxy(int, int);
int select_num(void);
void cursor_control(char, int *, int *, int *);
void game_play();
void player_move(char[N][M], int *, int *);
void missile_move(char[N][M]);
void enemy_move(char[N][M], int *);
void enemy_missile(char[N][M]);
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

void arr_init(char game_arr[N][M])
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			if (i < 8 && i > 4 && j > 20 && j < 40)
				game_arr[i][j] = 1;				/*enemy*/
			else if (j == M - 1)
				game_arr[i][j] = 3;
			else
				game_arr[i][j] = 0;
		}
	}
}

int arr_print(char game_arr[N][M], int *score)
{
	int i, j, cnt = 0;

	for (i = 0; i < N; i++)
	{

		for (j = 0; j < M; j++)
		{
			switch (game_arr[i][j]) {
			case 0:
				printf(" ");
				break;
			case 1:
				printf("@");	/*enemy*/
				cnt++;
				break;
			case 2:
				printf("!");		/*player_missile*/
				if (game_arr[i - 1][j] == 1 || game_arr[i - 1][j] == 4)
				{
					game_arr[i][j] = 0;
					game_arr[i - 1][j] = 0;
					*score += 10;
				}
				else
				{
					game_arr[i - 1][j] = 2;
					game_arr[i][j] = 0;
				}
				break;
			case 3:
				printf("┃");
				break;
			case 4:
				printf("$");		/*enemy_missile*/
				break;
			default:
				break;
			}

		}
		printf("\n");
	}
	gotoxy(80, 10);
	printf("현재 점수");
	gotoxy(80, 11);
	printf("%d", *score);
	return cnt;
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
	printf("☞");

	while (1)
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
		printf("☞");

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
	case 72:				/*up*/
		if (*y > 0)
			*y -= 1;
		break;

	case 80:
		*y += 1;			/*down*/
		break;

	case 75:				/*left*/
		if (*x > 0)
			*x -= 1;
		break;

	case 77:				/*right*/
		*x += 1;
		break;

	case 32:				/*SP*/
		*space = 1;
		break;

	default:
		break;
	}
}

void game_play()
{
	int x = M / 2, y = N, way = 0;		//x좌표, y좌표, 적이 움직이는 방향
	int score = 0, cnt;

	system("cls");
	gotoxy(x, y);
	printf("*^*");
	while (1)
	{	
		if(_kbhit())
			player_move(game_arr, &x, &y);
		enemy_move(game_arr, &way);
		enemy_missile(game_arr);
		gotoxy(0, 0);
		if (arr_print(game_arr, &score) == 0)
			break;
	}
}

void player_move(char game_arr[N][M], int *i, int *j)
{
	char cursor;
	int space = 0;

	gotoxy(*i, *j);
	printf("*^*");

	cursor = getch();
	gotoxy(*i, *j);
	printf("   ");
	cursor_control(cursor, i, j, &space);

	*j = N;

	if (*i >= M - 3)
		*i = M - 3;

	gotoxy(*i, *j);
	printf("*^*");

	if (space == 1)
		game_arr[*j - 1][*i + 1] = 2;
}

void missile_move(char game_arr[N][M])
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			if (game_arr[i][j] == 2)
			{
				if (game_arr[i - 1][j] == 1)
				{
					game_arr[i][j] = 0;
					game_arr[i - 1][j] = 0;
				}
				else
				{
					game_arr[i - 1][j] = 2;
					game_arr[i][j] = 0;
				}
			}
		}
	}
}

void enemy_move(char game_arr[N][M], int *w)
{
	int i, j, l = 0, r = 0;

	if (*w == 0)						/*왼쪽으로 이동*/
	{
		for (i = 0; i < N; i++)
		{
			for (j = 0; j <= M - 1; j++)		/*한칸씩 왼쪽으로 이동*/
			{
				if (game_arr[i][j] == 1)
				{
					game_arr[i][j - 1] = game_arr[i][j];
					game_arr[i][j] = 0;
				}
			}
		}
	}
	else if (*w == 1)			/*오른쪽으로 이동*/
	{
		for (i = 0; i < N; i++)
		{
			for (j = M - 2; j >= 0; j--)		/*한칸씩 오른쪽으로 이동*/
			{
				if (game_arr[i][j] == 1)
				{
					game_arr[i][j + 1] = game_arr[i][j];
					game_arr[i][j] = 0;
				}
			}
		}
	}

	for (i = N; i > 0; i--)					/*양 끝에 도착했는지 검사*/
	{
		if (game_arr[i][0] == 1)
			l = 1;
		else if (game_arr[i][M - 2] == 1)
			r = 1;
	}

	if (r == 1 || l == 1)
	{
		for (i = N; i > 0; i--)					/*양 끝에 도착하면 한칸 아래로 이동*/
		{
			for (j = 0; j < M - 1; j++)
			{
				if (game_arr[i][j] == 1)
				{
					game_arr[i + 1][j] = game_arr[i][j];
					game_arr[i][j] = 0;
				}
			}
		}
		if (r == 1)
			*w = 0;
		else if (l == 1)
			*w = 1;

	}
}

void enemy_missile(char game_arr[N][M])
{
	int i, j, k = 0;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			if (game_arr[i][j] == 4)
			{
				game_arr[i + 1][j] = 4;
				game_arr[i][j] = 0;
			}
		}
	}

	srand(time(NULL));
	for (k = 0; k < 50; k++)
	{
		i = rand() % N;
		j = rand() % M;
		if (game_arr[i][j] == 1 && game_arr[i + 1][j] != 1)
			game_arr[i + 1][j] = 4;
	}
}