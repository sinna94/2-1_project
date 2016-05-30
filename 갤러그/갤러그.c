#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define N 27
#define M 61

typedef struct save {
	char name[10];
	int score;
}save;

char game_arr[N][M];

void arr_init_1(char[N][M]);
void arr_init_2(char game_arr[N][M]);
void arr_init_3(char game_arr[N][M]);
int arr_print(char[N][M], int);
void menu(void);
void gotoxy(int, int);
int select_num(void);
void cursor_control(char, int *, int *, int *);
void game_play();
void player_move(char[N][M], int *, int *);
void missile_move(char[N][M], int *, int *);
int enemy_move(char[N][M], int *);
void enemy_missile(char[N][M]);
void enemy_missile_move(char game_arr[N][M]);
int player_die(char[N][M], int, int);
void save_score(int);
void print_score();
void boss_missile(char[N][M]);

int main()
{
	while (1)
	{
		menu();
		switch (select_num()) {
		case 1:
			game_play();
			break;
		case 2:
			print_score();
			break;
		case 3:
			exit(1);
		}
	}
}

void arr_init_0(char game_arr[N][M])
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
			game_arr[i][j] = 0; 
	}
}

void arr_init_1(char game_arr[N][M])
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

void arr_init_2(char game_arr[N][M])
{
	int i, j;
	srand(time(NULL));
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j += 3)
		{
			if (i < 9 && i > 3 && j > 20 && j < 50)
				game_arr[i][j] = 1;				/*enemy*/
			else if (j == M - 1)
				game_arr[i][j] = 3;
			else
				game_arr[i][j] = 0;
		}
	}
}

void arr_init_3(char game_arr[N][M])
{
	FILE *fp;
	
	int i, j;
	
	if ((fp = fopen("boss.txt", "r")) == NULL)
	{
		printf("파일이 열리지 않습니다.\n");
		exit(1);
	}

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
			fscanf(fp, "%c", &game_arr[i][j]);
	}

	fclose(fp);
}

int arr_print(char game_arr[N][M], int score)
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
			case '0':
				printf(" ");
				break;
			case 1:
				printf("@");	/*enemy*/
				cnt++;
				break;
			case 2:
				printf("!");		/*player_missile*/
				break;
			case 3:
				printf("┃");
				break;
			case '3':
				printf("┃");
				break;
			case 4:
				printf("$");		/*enemy_missile*/
				break;
			case '5':
				printf("*");		/*boss*/
				cnt++;
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
	printf("%d", score);
	return cnt;
}

void menu()
{
	int sel_num = 0;
	system("cls");
	gotoxy(30, 10);
	puts("1. game start");
	gotoxy(30, 11);
	puts("2. score");
	gotoxy(30, 12);
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
	int x = 28, y = 10, space = 0;
	char cursor;

	gotoxy(x, y);
	printf("☞");

	while (1)
	{
		cursor = getch();
		gotoxy(x, y);
		printf(" ");
		cursor_control(cursor, &x, &y, &space);

		x = 28;

		if ((y > 12))
			y -= 1;

		else if (y < 10)
			y += 1;

		gotoxy(x, y);
		printf("☞");

		if (space == 1)
			break;
	}

	switch (y) {
	case 10:
		return 1;
		break;

	case 11:
		return 2;
		break;

	case 12:
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
	int x = M / 2, y = N, way = 0;		/*x좌표, y좌표, 적이 움직이는 방향*/
	int score = 0, delay, hard, stage = 3, i;
	int end, hp = 200;

	system("cls");
	gotoxy(x, y);
	printf("*^*");
		
	for (i = 0; i < stage; i++)
	{
		switch (i)
		{
		case 0:
			system("cls");
			gotoxy(30, 10);
			printf("stage: 1");
			Sleep(1000);
			system("cls");
			arr_init_1(game_arr);
			break;
		case 1:
			system("cls");
			gotoxy(30, 10);
			printf("stage: 2");
			Sleep(1000);
			system("cls");
			arr_init_0(game_arr);
			arr_init_2(game_arr);
			break;
		case 2:
			system("cls");
			gotoxy(30, 10);
			printf("stage: 3");
			Sleep(1000);
			system("cls");
			arr_init_0(game_arr);
			arr_init_3(game_arr);
			break;
		}
	
		end = 0;
		hard = 5 - i;
		delay = 0;

		while (1)
		{
			gotoxy(0, 0);
			if (arr_print(game_arr, score) == 0)
				break;

			if (i == 2)
			{
				gotoxy(80, 13);
				printf("HP: %3d", hp);
			}

			if (player_die(game_arr, x, y) != 0)
			{
				end = 1;
				break;
			}

			if (_kbhit())
				player_move(game_arr, &x, &y);

			if (delay % hard == 0)
				if (enemy_move(game_arr, &way) == -1)
				{
					end = 1;
					break;
				}
			missile_move(game_arr, &score, &hp);
			enemy_missile(game_arr);
			boss_missile(game_arr);

			if (delay % hard == 0)
			{
				enemy_missile_move(game_arr);
	
			}
							
			delay++;

			if (hp == 0)
			{
				score += 200;
				break;
			}
			if (delay % 300 == 0 && hard > 1)
				hard--;
		}
		if (end == 1)
			break;
	}
	
	system("cls");
	gotoxy(30, 20);
	printf("게임 종료.");
	
	Sleep(3000);
	
	save_score(score);
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

void missile_move(char game_arr[N][M], int *score, int *hp)
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
					*score += 10;
				}
				else if (game_arr[i - 1][j] == 4)
				{
					game_arr[i][j] = 0;
					game_arr[i - 1][j] = 0;
				}
				else if (game_arr[i - 1][j] == '5')
				{
					game_arr[i][j] = 0;
					*score += 15;
					*hp -= 1;
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

int enemy_move(char game_arr[N][M], int *w)
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

	if (r == 1 || l == 1)						/*양 끝에 도착하면 한칸 아래로 이동*/
	{
		for (i = N; i > 0; i--)
		{
			for (j = 0; j < M - 1; j++)
			{
				if (game_arr[i][j] == 1)
				{
					if (game_arr[N][j] == 1)
						return -1;
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
	return 0;
}

void enemy_missile(char game_arr[N][M])
{
	int j, k, x;

	srand(time(NULL));
	for (k = 0; k < 3; k++)
	{
		j = rand() % M;
		for (x = N; x > 0; x--)
		{
			if (game_arr[x][j] == 1)
			{	
				game_arr[x + 1][j] = 4;
				break;
			}
		}
		
	}
}

void enemy_missile_move(char game_arr[N][M])
{
	int i, j;

	for (i = N; i > 0; i--)
	{
		for (j = M; j > 0; j--)
		{
			if (game_arr[i][j] == 4)
			{
				if (game_arr[i + 1][j] == 1 || game_arr[i + 1][j - 1] == 1 || game_arr[i + 1][j + 1] == 1)
					game_arr[i + 2][j] == 4;
				else
					game_arr[i + 1][j] = 4;
				game_arr[i][j] = 0;
			}
		}
	}
}

void boss_missile(char game_arr[N][M])
{
	int j, k, x;

	srand(time(NULL));
	for (k = 0; k < 4; k++)
	{
		j = rand() % M;
		
		for (x = N; x > 0; x--)
		{
			if (game_arr[x][j] == '5' )
			{
				game_arr[x + 1][j] = 4;
				break;
			}
		}

	}
}

int player_die(char game_arr[N][M], int x, int y)
{
	int i, j;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			if (game_arr[i][j] == 4 && y == i + 1 && (x == j - 2 || x == j - 1 || x == j))
				return -1;
		}
	}
	return 0;
}

void save_score(int score)
{
	
	FILE* fp;
	save file[11] = { NULL };
	save save;

	int i, j, k = 0;

	if ((fp = fopen("score.txt", "r")) == NULL)
	{
		printf("파일이 열리지 않습니다.\n");
		exit(1);
	}

	if (fp != NULL)
	{
		for (k = 0; k < 10; k++)
			if (fscanf(fp, "%s %d", file[k].name, &file[k].score) == EOF)
				break;
	}
	
	fclose(fp);

	if ((fp = fopen("score.txt", "w")) == NULL)
	{
		printf("파일이 열리지 않습니다.\n");
		exit(1);
	}
	gotoxy(30, 20);
	fputs("이름 입력: ", stdout);
	gets(file[k].name);
	file[k].score = score;
	k++;

	for (i = 1; i < k; i++)
	{
		save = file[i];
		for (j = i - 1; j >= 0 && file[j].score > file[j + 1].score; j--)
		{
			file[j + 1] = file[j];
			file[j] = save;
		}
	}
	
	for (i = k - 1; i >= 0; i--)
		fprintf(fp, "%s %d\n", file[i].name, file[i].score);
	
	fclose(fp);
}

void print_score()
{
	FILE * fp;
	save score[10] = { NULL };
	int i, j, y = 4;
	system("cls");
	gotoxy(30, y);
	printf("순위\t이름\t점수\n");
	if ((fp = fopen("score.txt", "r")) == NULL)
	{
		printf("파일이 열리지 않습니다.\n");
		exit(1);
	}

	for (i = 0; i < 10; i++)
	{
		fscanf(fp, "%s %d\n", score[i].name, &score[i].score);
		if (strcmp(score[i].name, "0") == 0 && score[i].score == 0)
			break;
	}

	for (j = 0; j < i; j++)
	{
		gotoxy(30, y + (j + 1)*2);
		printf("%d.\t %s \t %d\n", j + 1, score[j].name, score[j].score);
	}
	Sleep(5000);
	
	fclose(fp);
}
