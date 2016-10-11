#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define N 27
#define M 61

typedef struct save {												/*���� �����ϱ� ���� ����ü*/
	char name[10];
	int score;
}save;

char game_arr[N][M];												/*���� �迭*/

void arr_init_0(char [N][M]);										/*�迭 ��ü 0���� �ʱ�ȭ*/
void arr_init_1(char [N][M]);										/*1ź �迭 �ʱ�ȭ*/
void arr_init_2(char [N][M]);										/*2ź �迭 �ʱ�ȭ*/
void arr_init_3(char [N][M]);										/*3ź �迭 �ʱ�ȭ*/
int arr_print(char[N][M], int);										/*�迭 ���, ���� ���*/
void menu(void);													/*�޴� ���*/
void gotoxy(int, int);												/*��ǥ �̵� �Լ�*/
int select_num(void);												/*�޴� ���� ���� �Լ�*/
void cursor_control(char, int *, int *, int *);						/*Ű���� �Է� �Լ�*/
void game_play();													/*���� ���� �Լ�*/
void player_move(char[N][M], int *, int *);							/*�÷��̾� �̵� �Լ�*/
void missile_move(char[N][M], int *, int *);						/*�÷��̾� �̻��� �̵� �Լ�*/
int enemy_move(char[N][M], int *);									/*�� �̵� �Լ�*/
void enemy_missile(char[N][M]);										/*�� �̻��� ���� �Լ�*/
void enemy_missile_move(char [N][M]);								/*�� �̻��� �̵� �Լ�*/
int player_die(char[N][M], int, int);								/*�� �̻��Ͽ� �÷��̾� �ǰ� ���� �Լ�*/
void save_score(int);												/*���� ����, ���� �Լ�*/
void print_score();													/*���� ��� �Լ�*/
void boss_missile(char[N][M]);										/*3ź ���� �̻��� ��� �Լ�*/

int main()
{
	system("mode con: cols=90 lines=30");							/*�ܼ� â ũ�� ����*/
	while (1)														/*���� ������ �޴� ������ ���� �ݺ���*/
	{
		menu();														/*�޴� ���*/
		switch (select_num()) {										/*���� �����ϴ� �Լ�*/
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

void arr_init_0(char game_arr[N][M])								/*��� �迭�� 0 ������ �ʱ�ȭ �ϴ� �Լ�*/
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
			game_arr[i][j] = 0; 
	}
}

void arr_init_1(char game_arr[N][M])								/*1ź���� �迭 �ʱ�ȭ �ϴ� �Լ�*/
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			if (i < 8 && i > 4 && j > 20 && j < 40)
				game_arr[i][j] = 1;				/*��*/
			else if (j == M - 1)
				game_arr[i][j] = 3;				/*ĭ����*/
			else
				game_arr[i][j] = 0;				/*�����*/

		}
	}
}

void arr_init_2(char game_arr[N][M])								/*2ź���� �迭 �ʱ�ȭ �ϴ� �Լ�*/
{
	int i, j;
	srand(time(NULL));
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j += 3)
		{
			if (i < 9 && i > 3 && j > 20 && j < 50)
				game_arr[i][j] = 1;				/*��*/
			else if (j == M - 1)
				game_arr[i][j] = 3;				/*ĭ����*/
			else
				game_arr[i][j] = 0;				/*�����*/
		}
	}
}

void arr_init_3(char game_arr[N][M])								/*3ź���� �迭 �ʱ�ȭ �ϴ� �Լ�*/
{
	FILE *fp;
	
	int i, j;
	
	if ((fp = fopen("boss.txt", "r")) == NULL)						/*boss �ؽ�Ʈ ������ �б�*/
	{
		printf("������ ������ �ʽ��ϴ�.\n");
		exit(1);
	}

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
			fscanf(fp, "%c", &game_arr[i][j]);						/*game_arr �迭�� boss ������ ���� �ֱ�*/
	}

	fclose(fp);
}

int arr_print(char game_arr[N][M], int score)						/*game_arr �迭�� ������ ����ϴ� �Լ�*/
{
	int i, j, cnt = 0;			/*cnt�� ���糲���ִ� ���� ���� ī��Ʈ �ϱ� ���� ����*/

	for (i = 0; i < N; i++)
	{

		for (j = 0; j < M; j++)
		{
			switch (game_arr[i][j]) {
			case 0:				/*�� ����*/
				printf(" ");
				break;
			case '0':			/*boss ���� �о��� ���� �� ����*/
				printf(" ");
				break;
			case 1:				/*��*/
				printf("@");	
				cnt++;			/*���� ����Ҷ����� 1�� ����*/
				break;
			case 2:				/*�÷��̾��� �̻���*/
				printf("!");		
				break;
			case 3:				/*ĭ����*/
				printf("��");
				break;
			case '3':			/*boss ���� �о��� ���� ĭ����*/
				printf("��");
				break;
			case 4:				/*���� �̻���*/
				printf("$");		
				break;
			case '5':			/*boss*/
				printf("*");		
				cnt++;			/*cnt�� 0�̸� ������ ��������� �����ϵ��� �־��־���*/
				break;
			default:
				break;
			}

		}
		printf("\n");
	}
	gotoxy(80, 10);
	printf("���� ����");
	gotoxy(80, 11);
	printf("%d", score);		/*���� ���� ���*/
	return cnt;					/*cnt �� ��ȯ*/
}

void menu()
{
	int sel_num = 0;
	system("cls");				/*ȭ�� �ʱ�ȭ*/
	gotoxy(30, 10);
	puts("1. game start");
	gotoxy(30, 11);
	puts("2. score");
	gotoxy(30, 12);
	puts("3. exit");
}

void gotoxy(int x, int y)												/*��ǥ �̵� �Լ�*/
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

int select_num(void)													/*�޴� ���� ���� �Լ�*/
{
	int x = 28, y = 10, space = 0;					/*x, y�� �޴��� ��µ� ��ǥ*/
	char cursor;

	gotoxy(x, y);
	printf("��");

	while (1)
	{
		cursor = getch();							/*Ű���忡�� �Է� ����*/
		gotoxy(x, y);
		printf(" ");
		cursor_control(cursor, &x, &y, &space);		/*Ű���忡�� �Է� ���� cursor ������ x, y, space �� ����*/

		x = 28;										/*�޴��� x���� ���� �Ǿ� ������ x ���� �׻� 28�� ������*/

		if ((y > 12))								/*y���� �ִ� �� ����*/
			y -= 1;

		else if (y < 10)							/*y���� �ּ� �� ����*/
			y += 1;

		gotoxy(x, y);
		printf("��");

		if (space == 1)								/*���ý� �ݺ��� Ż��*/
			break;
	}

	switch (y) {									/*y ���� ���� ����*/
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
	case 72:				/*����Ű ��*/
		if (*y > 0)
			*y -= 1;
		break;

	case 80:
		*y += 1;			/*����Ű �Ʒ�*/
		break;

	case 75:				/*����Ű ����*/
		if (*x > 0)
			*x -= 1;
		break;

	case 77:				/*����Ű ������*/
		*x += 1;
		break;

	case 32:				/*�����̽� ��*/
		*space = 1;
		break;

	default:
		break;
	}
}

void game_play()
{
	int x = M / 2, y = N, way = 0;		/*x��ǥ, y��ǥ, ���� �����̴� ����*/
	int score = 0, delay, hard, stage = 3, i;	/*score�� ���� ����, delay�� hard�� ���̵� ������ ���� ����, 
												stage�� ���� ���������� ��*/
	int end, hp = 200;					/*end�� ������ �й� �ߴ��� Ȯ�� �ϴ� ����, hp�� boss�� ü�� ����*/

	system("cls");
	gotoxy(x, y);
	printf("*^*");
		
	for (i = 0; i < stage; i++)				/*�������� ������ ���� �ݺ���*/
	{
		switch (i)							/*���������� ���� �迭 �ʱ�ȭ*/
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
		hard = 5 - i;					/*���������� ���� �⺻ ���̵�*/
		delay = 0;						/*�������� ���� �� delay �� �ʱ�ȭ*/

		while (1)						/*���� �÷��̸� ���� �ݺ���*/
		{
			gotoxy(0, 0);
			if (arr_print(game_arr, score) == 0)			/*�迭 ����� ���� ���, ���� ���� ���� �� ��ȯ*/
				break;										/*��ȯ ���� 0�̶�� �ݺ��� Ż��*/

			if (i == 2)										/*3��° �������� ���� �۵�*/
			{
				gotoxy(80, 13);
				printf("HP: %3d", hp);						/*boss�� ���� ü�� ���*/
			}

			if (player_die(game_arr, x, y) != 0)			/*�÷��̾� ��� üũ �Լ�*/
			{
				end = 1;									/*�й踦 Ȯ���ϴ� ������ 1�� ����*/
				break;
			}
															/*Ű���忡 �Է��� �������� Ȯ���ϴ� �Լ�*/
			if (_kbhit())									/*Ű���� �Է��� ��� �Ʒ� �Լ��� �۵��ϵ��� ��*/
				player_move(game_arr, &x, &y);				/*�÷��̾� �̵� �Լ�*/

			if (delay % hard == 0)							/*���� ���̵� ������ ���� ���ǹ�,delay ���� hard ������ ���� �������� 0�϶��� �۵�*/
				if (enemy_move(game_arr, &way) == -1)		/*���� ������ �Լ�, ���� �� �Ʒ��� �����ϸ� -1�� ��ȯ*/
				{			
					end = 1;								/*�й踦 Ȯ���ϴ� ������ 1�� ����*/
					break;
				}
			missile_move(game_arr, &score, &hp);			/*�÷��̾� �̻��� �̵� �Լ�*/
			enemy_missile(game_arr);						/*�� �̻��� ���� �Լ�*/
			if(i == 2)										/*3��° �������������� �۵�*/
				boss_missile(game_arr);						/*boss �̻��� ���� �Լ�*/
	
			if (delay % hard == 0)							/*���� ���̵� ������ ���� ���ǹ�,delay ���� hard ������ ���� �������� 0�϶��� �۵�*/
				enemy_missile_move(game_arr);				/*�� �̻��� �̵� �Լ�*/
			
			delay++;										/*delay �� 1 ����*/

			if (hp == 0)									/*boss ü���� 0�� ���� ���*/
			{
				score += 200;
				break;
			}
			if (delay % 300 == 0 && hard > 1)				/*delay�� 300�� ����̰� hard�� 1���� Ŭ ���� �۵�*/
				hard--;										/*hard ���� 1 ����*/
		}
		if (end == 1)										/*���� �й� �� for�� Ż��*/
			break;
	}
	system("cls");
	gotoxy(30, 20);
	printf("���� ����.");
	Sleep(3000);											/*���� ������ 3�ʰ� ����*/
	save_score(score);										/*������ �����ϴ� �Լ�*/
}

void player_move(char game_arr[N][M], int *i, int *j)		/*�÷��̾� �̵� �Լ�*/
{
	char cursor;
	int space = 0;

	gotoxy(*i, *j);
	printf("*^*");

	cursor = getch();										/*Ű����� �Է� ����*/
	gotoxy(*i, *j);
	printf("   ");
	cursor_control(cursor, i, j, &space);					/*�Է� ���� cursor ������ i, j, space �� ����*/

	*j = N;													/*�÷��̾�� �׻� �� �Ʒ� �־�� ������ j�� ����*/

	if (*i >= M - 3)										/*�÷��̾ *^* ������� ����� 3�̱� ������ */
		*i = M - 3;

	gotoxy(*i, *j);
	printf("*^*");

	if (space == 1)											/*space ���� 1�� ���*/
		game_arr[*j - 1][*i + 1] = 2;						/*�÷��̾� �߾� ��ġ�� �ٷ� �� �迭�� 2 �� ����*/
}

void missile_move(char game_arr[N][M], int *score, int *hp)	/*�÷��̾��� �̻��� �̵� �Լ�*/
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			if (game_arr[i][j] == 2)						/*���� �迭�� ���� 2(�÷��̾� �̻���)�� ���*/
			{
				if (game_arr[i - 1][j] == 1)				/*�ٷ� ���� ���� ���� 1(��)�� ���*/
				{
					game_arr[i][j] = 0;						/*���� �迭�� �ٷ� ���� ���� ���� 0���� ����*/
					game_arr[i - 1][j] = 0;
					*score += 10;							/*score 10�� �߰�*/
				}
				else if (game_arr[i - 1][j] == 4)			/*�ٷ� ���� ���� ���� 4(�� �̻���)�� ���*/
				{
					game_arr[i][j] = 0;						/*���� �迭�� �ٷ� ���� ���� ���� 0���� ����*/
					game_arr[i - 1][j] = 0;
				}
				else if (game_arr[i - 1][j] == '5')			/*�ٷ� ���� ���� ���� '5'(boss)�� ���*/
				{
					game_arr[i][j] = 0;						/*���� �迭 �� 0���� ����*/
					*score += 15;							/*score 15�� �߰�*/
					*hp -= 1;								/*boss�� ü�� 1 ����*/
				}
				else										/*�̻����� �ٷ� ���� ������ �̵�*/
				{	
					game_arr[i - 1][j] = 2;
					game_arr[i][j] = 0;
				}
			}
		}
	}
}

int enemy_move(char game_arr[N][M], int *w)					/*�� �̵� �Լ�*/
{
	int i, j, l = 0, r = 0;

	if (*w == 0)						/*�������� �̵�*/
	{
		for (i = 0; i < N; i++)
		{
			for (j = 0; j <= M - 1; j++)		/*��ĭ�� �������� �̵�*/
			{
				if (game_arr[i][j] == 1)
				{
					game_arr[i][j - 1] = game_arr[i][j];
					game_arr[i][j] = 0;
				}
			}
		}
	}
	else if (*w == 1)			/*���������� �̵�*/
	{
		for (i = 0; i < N; i++)
		{
			for (j = M - 2; j >= 0; j--)		/*��ĭ�� ���������� �̵�*/
			{
				if (game_arr[i][j] == 1)
				{
					game_arr[i][j + 1] = game_arr[i][j];
					game_arr[i][j] = 0;
				}
			}
		}
	}

	for (i = N; i > 0; i--)					/*�� ���� �����ߴ��� �˻�*/
	{
		if (game_arr[i][0] == 1)
			l = 1;
		else if (game_arr[i][M - 2] == 1)
			r = 1;
	}

	if (r == 1 || l == 1)						/*�� ���� �����ϸ� ��ĭ �Ʒ��� �̵�*/
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

void enemy_missile(char game_arr[N][M])						/*�� �̻��� ���� �Լ�*/
{
	int j, k, x;

	srand(time(NULL));
	for (k = 0; k < 3; k++)
	{
		j = rand() % M;										/*�������� j �� ����*/
		for (x = N; x > 0; x--)								/*�Ʒ� �����*/
		{
			if (game_arr[x][j] == 1)						/*���� �迭�� ���� 1(��)�� ���*/
			{	
				game_arr[x + 1][j] = 4;						/*�ٷ� �Ʒ� �� ���� 4(�� �̻���)�� ����*/
				break;
			}
		}
		
	}
}

void enemy_missile_move(char game_arr[N][M])				/*�� �̻��� �̵� �Լ�*/
{
	int i, j;

	for (i = N; i > 0; i--)
	{
		for (j = M; j > 0; j--)
		{
			if (game_arr[i][j] == 4)						/*���� �迭 ���� 4(�� �̻���)�� ���)*/
			{												/*���� �� ��, ��, �߰� ���� �ϳ��� 1(��)�� ���*/
				if (game_arr[i + 1][j] == 1 || game_arr[i + 1][j - 1] == 1 || game_arr[i + 1][j + 1] == 1)
					game_arr[i + 2][j] == 4;				/*�̻����� 2�� �Ʒ��� �̵�*/
				else										/*�̻����� �ٷ� �Ʒ� ������ �̵�*/
					game_arr[i + 1][j] = 4;
				game_arr[i][j] = 0;
			}
		}
	}
}

void boss_missile(char game_arr[N][M])						/*boss �̻��� ���� �Լ�*/
{
	int j, k, x;

	srand(time(NULL));
	for (k = 0; k < 4; k++)
	{
		j = rand() % M;										/*�������� j �� ����*/
		
		for (x = N; x > 0; x--)								/*�Ʒ� �����*/
		{
			if (game_arr[x][j] == '5' )						/*���� �迭�� ���� '5'(boss)�� ���*/
			{
				game_arr[x + 1][j] = 4;						/*�ٷ� �Ʒ� �� ���� 4(�� �̻���)�� ����*/
				break;
			}
		}

	}
}

int player_die(char game_arr[N][M], int x, int y)			/*�÷��̾� ���� Ȯ�� �Լ�*/
{
	int i, j;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{													/*�÷��̾� �ٷ� ���� �迭���� 4(�� �̻���) �̰� *^* �� �ϳ��� �� �̻��Ͽ� �¾��� ���*/
			if (game_arr[i][j] == 4 && y == i + 1 && (x == j - 2 || x == j - 1 || x == j))
				return -1;									/*-1�� ��ȯ*/
		}
	}
	return 0;												/*�ƹ��ϵ� ���� ��� 0�� ��ȯ*/
}

void save_score(int score)									/*���� ���� �Լ�*/
{
	
	FILE* fp;
	save file[11] = { NULL };								/*���� ������ ���� ����ü �迭*/
	save save;

	int i, j, k = 0;

	if ((fp = fopen("score.txt", "r")) == NULL);			/*�б� ���� score.txt ����*/

	else {													/*������ �ִٸ�*/
		if (fp != NULL)
		{
			for (k = 0; k < 10; k++)						/*score.txt �� �ִ� ���� �迭 file�� �Է�*/
				if (fscanf(fp, "%s %d", file[k].name, &file[k].score) == EOF)
					break;
		}

		fclose(fp);											/*���� �ݱ�*/
	}

	if ((fp = fopen("score.txt", "w")) == NULL)				/*���� ���� score.txt ����*/
	{
		printf("������ ������ �ʽ��ϴ�.\n");
		exit(1);
	}
	gotoxy(30, 20);
	fputs("�̸� �Է�: ", stdout);
	gets(file[k].name);										/*�̸� �Է� �ް�*/
	file[k].score = score;									/*���� ����*/
	k++;

	for (i = 1; i < k; i++)									/*score.txt���� �޾ƿ� ���� ���ο� �̸�, ������ �������ķ� ����*/
	{
		save = file[i];
		for (j = i - 1; j >= 0 && file[j].score > file[j + 1].score; j--)
		{
			file[j + 1] = file[j];
			file[j] = save;
		}
	}
	
	for (i = k - 1; i >= 0; i--)								/*�ִ� 10���� �������� ���Ͽ� ����*/
		fprintf(fp, "%s %d\n", file[i].name, file[i].score);
	
	fclose(fp);												/*���� �ݱ�*/
	print_score();											/*���� ��� �Լ�*/
}

void print_score()											/*���� ��� �Լ�*/
{
	FILE * fp;
	save score[10] = { NULL };								/*���� ����� ���� ����ü �迭*/
	int i, j, y = 4;
	system("cls");
	gotoxy(30, y);
	printf("����\t�̸�\t����\n");
	if ((fp = fopen("score.txt", "r")) == NULL)				/*�б� ���� score.txt ���� ����*/
	{
		printf("������ �����ϴ�.\n");
		exit(1);
	}

	for (i = 0; i < 10; i++)								/*�迭 score�� score.txt���� �о�� �� ����*/
	{
		fscanf(fp, "%s %d\n", score[i].name, &score[i].score);
		if (strcmp(score[i].name, "0") == 0 && score[i].score == 0)
			break;
	}

	for (j = 0; j < i; j++)									/*�迭 score ���*/
	{
		gotoxy(30, y + (j + 1)*2);
		printf("%d.\t %s \t %d\n", j + 1, score[j].name, score[j].score);
	}
	Sleep(5000);											/*5�� ����*/
	
	fclose(fp);												/*���� �ݱ�*/
}
