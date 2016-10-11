#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define N 27
#define M 61

typedef struct save {												/*점수 저장하기 위한 구조체*/
	char name[10];
	int score;
}save;

char game_arr[N][M];												/*게임 배열*/

void arr_init_0(char [N][M]);										/*배열 전체 0으로 초기화*/
void arr_init_1(char [N][M]);										/*1탄 배열 초기화*/
void arr_init_2(char [N][M]);										/*2탄 배열 초기화*/
void arr_init_3(char [N][M]);										/*3탄 배열 초기화*/
int arr_print(char[N][M], int);										/*배열 출력, 점수 출력*/
void menu(void);													/*메뉴 출력*/
void gotoxy(int, int);												/*좌표 이동 함수*/
int select_num(void);												/*메뉴 숫자 선택 함수*/
void cursor_control(char, int *, int *, int *);						/*키보드 입력 함수*/
void game_play();													/*게임 진행 함수*/
void player_move(char[N][M], int *, int *);							/*플레이어 이동 함수*/
void missile_move(char[N][M], int *, int *);						/*플레이어 미사일 이동 함수*/
int enemy_move(char[N][M], int *);									/*적 이동 함수*/
void enemy_missile(char[N][M]);										/*적 미사일 생성 함수*/
void enemy_missile_move(char [N][M]);								/*적 미사일 이동 함수*/
int player_die(char[N][M], int, int);								/*적 미사일에 플레이어 피격 판정 함수*/
void save_score(int);												/*점수 정렬, 저장 함수*/
void print_score();													/*점수 출력 함수*/
void boss_missile(char[N][M]);										/*3탄 보스 미사일 출력 함수*/

int main()
{
	system("mode con: cols=90 lines=30");							/*콘솔 창 크기 조절*/
	while (1)														/*종료 전까지 메뉴 선택을 위한 반복문*/
	{
		menu();														/*메뉴 출력*/
		switch (select_num()) {										/*숫자 선택하는 함수*/
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

void arr_init_0(char game_arr[N][M])								/*모든 배열을 0 값으로 초기화 하는 함수*/
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
			game_arr[i][j] = 0; 
	}
}

void arr_init_1(char game_arr[N][M])								/*1탄으로 배열 초기화 하는 함수*/
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			if (i < 8 && i > 4 && j > 20 && j < 40)
				game_arr[i][j] = 1;				/*적*/
			else if (j == M - 1)
				game_arr[i][j] = 3;				/*칸막이*/
			else
				game_arr[i][j] = 0;				/*빈공간*/

		}
	}
}

void arr_init_2(char game_arr[N][M])								/*2탄으로 배열 초기화 하는 함수*/
{
	int i, j;
	srand(time(NULL));
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j += 3)
		{
			if (i < 9 && i > 3 && j > 20 && j < 50)
				game_arr[i][j] = 1;				/*적*/
			else if (j == M - 1)
				game_arr[i][j] = 3;				/*칸막이*/
			else
				game_arr[i][j] = 0;				/*빈공간*/
		}
	}
}

void arr_init_3(char game_arr[N][M])								/*3탄으로 배열 초기화 하는 함수*/
{
	FILE *fp;
	
	int i, j;
	
	if ((fp = fopen("boss.txt", "r")) == NULL)						/*boss 텍스트 파일을 읽기*/
	{
		printf("파일이 열리지 않습니다.\n");
		exit(1);
	}

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
			fscanf(fp, "%c", &game_arr[i][j]);						/*game_arr 배열에 boss 파일의 값을 넣기*/
	}

	fclose(fp);
}

int arr_print(char game_arr[N][M], int score)						/*game_arr 배열과 점수를 출력하는 함수*/
{
	int i, j, cnt = 0;			/*cnt는 현재남아있는 적의 수를 카운트 하기 위한 변수*/

	for (i = 0; i < N; i++)
	{

		for (j = 0; j < M; j++)
		{
			switch (game_arr[i][j]) {
			case 0:				/*빈 공간*/
				printf(" ");
				break;
			case '0':			/*boss 파일 읽었을 때의 빈 공간*/
				printf(" ");
				break;
			case 1:				/*적*/
				printf("@");	
				cnt++;			/*적을 출력할때마다 1씩 증가*/
				break;
			case 2:				/*플레이어의 미사일*/
				printf("!");		
				break;
			case 3:				/*칸막이*/
				printf("┃");
				break;
			case '3':			/*boss 파일 읽었을 때의 칸막이*/
				printf("┃");
				break;
			case 4:				/*적의 미사일*/
				printf("$");		
				break;
			case '5':			/*boss*/
				printf("*");		
				cnt++;			/*cnt가 0이면 게임이 종료됨으로 증가하도록 넣어주었음*/
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
	printf("%d", score);		/*현재 점수 출력*/
	return cnt;					/*cnt 값 반환*/
}

void menu()
{
	int sel_num = 0;
	system("cls");				/*화면 초기화*/
	gotoxy(30, 10);
	puts("1. game start");
	gotoxy(30, 11);
	puts("2. score");
	gotoxy(30, 12);
	puts("3. exit");
}

void gotoxy(int x, int y)												/*좌표 이동 함수*/
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

int select_num(void)													/*메뉴 숫자 선택 함수*/
{
	int x = 28, y = 10, space = 0;					/*x, y는 메뉴가 출력된 좌표*/
	char cursor;

	gotoxy(x, y);
	printf("☞");

	while (1)
	{
		cursor = getch();							/*키보드에서 입력 받음*/
		gotoxy(x, y);
		printf(" ");
		cursor_control(cursor, &x, &y, &space);		/*키보드에서 입력 받은 cursor 값으로 x, y, space 값 변경*/

		x = 28;										/*메뉴의 x값은 유지 되야 함으로 x 값은 항상 28을 유지함*/

		if ((y > 12))								/*y값의 최대 값 설정*/
			y -= 1;

		else if (y < 10)							/*y값의 최소 값 설정*/
			y += 1;

		gotoxy(x, y);
		printf("☞");

		if (space == 1)								/*선택시 반복문 탈출*/
			break;
	}

	switch (y) {									/*y 값에 따라서 선택*/
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
	case 72:				/*방향키 위*/
		if (*y > 0)
			*y -= 1;
		break;

	case 80:
		*y += 1;			/*방향키 아래*/
		break;

	case 75:				/*방향키 왼쪽*/
		if (*x > 0)
			*x -= 1;
		break;

	case 77:				/*방향키 오른쪽*/
		*x += 1;
		break;

	case 32:				/*스페이스 바*/
		*space = 1;
		break;

	default:
		break;
	}
}

void game_play()
{
	int x = M / 2, y = N, way = 0;		/*x좌표, y좌표, 적이 움직이는 방향*/
	int score = 0, delay, hard, stage = 3, i;	/*score는 현재 점수, delay와 hard는 난이도 설정을 위한 변수, 
												stage는 최종 스테이지의 수*/
	int end, hp = 200;					/*end는 게임이 패배 했는지 확인 하는 변수, hp는 boss의 체력 변수*/

	system("cls");
	gotoxy(x, y);
	printf("*^*");
		
	for (i = 0; i < stage; i++)				/*스테이지 구현을 위한 반복문*/
	{
		switch (i)							/*스테이지에 따른 배열 초기화*/
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
		hard = 5 - i;					/*스테이지에 따른 기본 난이도*/
		delay = 0;						/*스테이지 시작 전 delay 값 초기화*/

		while (1)						/*게임 플레이를 위한 반복문*/
		{
			gotoxy(0, 0);
			if (arr_print(game_arr, score) == 0)			/*배열 내용과 점수 출력, 현재 남의 적의 수 반환*/
				break;										/*반환 값이 0이라면 반복문 탈출*/

			if (i == 2)										/*3번째 스테이지 때만 작동*/
			{
				gotoxy(80, 13);
				printf("HP: %3d", hp);						/*boss의 현재 체력 출력*/
			}

			if (player_die(game_arr, x, y) != 0)			/*플레이어 사망 체크 함수*/
			{
				end = 1;									/*패배를 확인하는 변수를 1로 변경*/
				break;
			}
															/*키보드에 입력이 들어오는지 확인하는 함수*/
			if (_kbhit())									/*키보드 입력이 없어도 아래 함수가 작동하도록 함*/
				player_move(game_arr, &x, &y);				/*플레이어 이동 함수*/

			if (delay % hard == 0)							/*게임 난이도 조절을 위한 조건문,delay 값을 hard 값으로 나눈 나머지가 0일때만 작동*/
				if (enemy_move(game_arr, &way) == -1)		/*적의 움직임 함수, 적이 맨 아래에 도착하면 -1을 반환*/
				{			
					end = 1;								/*패배를 확인하는 변수를 1로 변경*/
					break;
				}
			missile_move(game_arr, &score, &hp);			/*플레이어 미사일 이동 함수*/
			enemy_missile(game_arr);						/*적 미사일 생성 함수*/
			if(i == 2)										/*3번째 스테이지에서만 작동*/
				boss_missile(game_arr);						/*boss 미사일 생성 함수*/
	
			if (delay % hard == 0)							/*게임 난이도 조절을 위한 조건문,delay 값을 hard 값으로 나눈 나머지가 0일때만 작동*/
				enemy_missile_move(game_arr);				/*적 미사일 이동 함수*/
			
			delay++;										/*delay 값 1 증가*/

			if (hp == 0)									/*boss 체력이 0이 됐을 경우*/
			{
				score += 200;
				break;
			}
			if (delay % 300 == 0 && hard > 1)				/*delay가 300의 배수이고 hard가 1보다 클 때만 작동*/
				hard--;										/*hard 값을 1 감소*/
		}
		if (end == 1)										/*게임 패배 시 for문 탈출*/
			break;
	}
	system("cls");
	gotoxy(30, 20);
	printf("게임 종료.");
	Sleep(3000);											/*게임 종료후 3초간 정지*/
	save_score(score);										/*점수를 저장하는 함수*/
}

void player_move(char game_arr[N][M], int *i, int *j)		/*플레이어 이동 함수*/
{
	char cursor;
	int space = 0;

	gotoxy(*i, *j);
	printf("*^*");

	cursor = getch();										/*키보드로 입력 받음*/
	gotoxy(*i, *j);
	printf("   ");
	cursor_control(cursor, i, j, &space);					/*입력 받은 cursor 값으로 i, j, space 값 변경*/

	*j = N;													/*플레이어는 항상 맨 아래 있어야 함으로 j값 고정*/

	if (*i >= M - 3)										/*플레이어가 *^* 모양으로 사이즈가 3이기 때문에 */
		*i = M - 3;

	gotoxy(*i, *j);
	printf("*^*");

	if (space == 1)											/*space 값이 1인 경우*/
		game_arr[*j - 1][*i + 1] = 2;						/*플레이어 중앙 위치의 바로 위 배열에 2 값 저장*/
}

void missile_move(char game_arr[N][M], int *score, int *hp)	/*플레이어의 미사일 이동 함수*/
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			if (game_arr[i][j] == 2)						/*현재 배열의 값이 2(플레이어 미사일)일 경우*/
			{
				if (game_arr[i - 1][j] == 1)				/*바로 위의 행의 값이 1(적)일 경우*/
				{
					game_arr[i][j] = 0;						/*현재 배열과 바로 위의 행의 값을 0으로 변경*/
					game_arr[i - 1][j] = 0;
					*score += 10;							/*score 10점 추가*/
				}
				else if (game_arr[i - 1][j] == 4)			/*바로 위의 행의 값이 4(적 미사일)일 경우*/
				{
					game_arr[i][j] = 0;						/*현재 배열과 바로 위의 행의 값을 0으로 변경*/
					game_arr[i - 1][j] = 0;
				}
				else if (game_arr[i - 1][j] == '5')			/*바로 위의 행의 값이 '5'(boss)일 경우*/
				{
					game_arr[i][j] = 0;						/*현재 배열 값 0으로 변경*/
					*score += 15;							/*score 15점 추가*/
					*hp -= 1;								/*boss의 체력 1 감소*/
				}
				else										/*미사일을 바로 위의 행으로 이동*/
				{	
					game_arr[i - 1][j] = 2;
					game_arr[i][j] = 0;
				}
			}
		}
	}
}

int enemy_move(char game_arr[N][M], int *w)					/*적 이동 함수*/
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

void enemy_missile(char game_arr[N][M])						/*적 미사일 생성 함수*/
{
	int j, k, x;

	srand(time(NULL));
	for (k = 0; k < 3; k++)
	{
		j = rand() % M;										/*랜덤으로 j 값 변경*/
		for (x = N; x > 0; x--)								/*아래 행부터*/
		{
			if (game_arr[x][j] == 1)						/*현재 배열의 값이 1(적)일 경우*/
			{	
				game_arr[x + 1][j] = 4;						/*바로 아래 행 값을 4(적 미사일)로 변경*/
				break;
			}
		}
		
	}
}

void enemy_missile_move(char game_arr[N][M])				/*적 미사일 이동 함수*/
{
	int i, j;

	for (i = N; i > 0; i--)
	{
		for (j = M; j > 0; j--)
		{
			if (game_arr[i][j] == 4)						/*현재 배열 값이 4(적 미사일)일 경우)*/
			{												/*다음 행 좌, 우, 중간 열중 하나가 1(적)일 경우*/
				if (game_arr[i + 1][j] == 1 || game_arr[i + 1][j - 1] == 1 || game_arr[i + 1][j + 1] == 1)
					game_arr[i + 2][j] == 4;				/*미사일을 2행 아래로 이동*/
				else										/*미사일을 바로 아래 행으로 이동*/
					game_arr[i + 1][j] = 4;
				game_arr[i][j] = 0;
			}
		}
	}
}

void boss_missile(char game_arr[N][M])						/*boss 미사일 생성 함수*/
{
	int j, k, x;

	srand(time(NULL));
	for (k = 0; k < 4; k++)
	{
		j = rand() % M;										/*랜덤으로 j 값 변경*/
		
		for (x = N; x > 0; x--)								/*아래 행부터*/
		{
			if (game_arr[x][j] == '5' )						/*현재 배열의 값이 '5'(boss)일 경우*/
			{
				game_arr[x + 1][j] = 4;						/*바로 아래 행 값을 4(적 미사일)로 변경*/
				break;
			}
		}

	}
}

int player_die(char game_arr[N][M], int x, int y)			/*플레이어 죽음 확인 함수*/
{
	int i, j;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{													/*플레이어 바로 위의 배열값이 4(적 미사일) 이고 *^* 이 하나라도 적 미사일에 맞았을 경우*/
			if (game_arr[i][j] == 4 && y == i + 1 && (x == j - 2 || x == j - 1 || x == j))
				return -1;									/*-1일 반환*/
		}
	}
	return 0;												/*아무일도 없을 경우 0을 반환*/
}

void save_score(int score)									/*점수 저장 함수*/
{
	
	FILE* fp;
	save file[11] = { NULL };								/*점수 정렬을 위한 구조체 배열*/
	save save;

	int i, j, k = 0;

	if ((fp = fopen("score.txt", "r")) == NULL);			/*읽기 모드로 score.txt 열기*/

	else {													/*파일이 있다면*/
		if (fp != NULL)
		{
			for (k = 0; k < 10; k++)						/*score.txt 에 있는 값을 배열 file에 입력*/
				if (fscanf(fp, "%s %d", file[k].name, &file[k].score) == EOF)
					break;
		}

		fclose(fp);											/*파일 닫기*/
	}

	if ((fp = fopen("score.txt", "w")) == NULL)				/*쓰기 모드로 score.txt 열기*/
	{
		printf("파일이 열리지 않습니다.\n");
		exit(1);
	}
	gotoxy(30, 20);
	fputs("이름 입력: ", stdout);
	gets(file[k].name);										/*이름 입력 받고*/
	file[k].score = score;									/*점수 저장*/
	k++;

	for (i = 1; i < k; i++)									/*score.txt에서 받아온 값과 새로운 이름, 점수를 삽입정렬로 정렬*/
	{
		save = file[i];
		for (j = i - 1; j >= 0 && file[j].score > file[j + 1].score; j--)
		{
			file[j + 1] = file[j];
			file[j] = save;
		}
	}
	
	for (i = k - 1; i >= 0; i--)								/*최대 10개의 값까지만 파일에 저장*/
		fprintf(fp, "%s %d\n", file[i].name, file[i].score);
	
	fclose(fp);												/*파일 닫기*/
	print_score();											/*점수 출력 함수*/
}

void print_score()											/*점수 출력 함수*/
{
	FILE * fp;
	save score[10] = { NULL };								/*점수 출력을 위한 구조체 배열*/
	int i, j, y = 4;
	system("cls");
	gotoxy(30, y);
	printf("순위\t이름\t점수\n");
	if ((fp = fopen("score.txt", "r")) == NULL)				/*읽기 모드로 score.txt 파일 열기*/
	{
		printf("파일이 없습니다.\n");
		exit(1);
	}

	for (i = 0; i < 10; i++)								/*배열 score에 score.txt에서 읽어온 값 저장*/
	{
		fscanf(fp, "%s %d\n", score[i].name, &score[i].score);
		if (strcmp(score[i].name, "0") == 0 && score[i].score == 0)
			break;
	}

	for (j = 0; j < i; j++)									/*배열 score 출력*/
	{
		gotoxy(30, y + (j + 1)*2);
		printf("%d.\t %s \t %d\n", j + 1, score[j].name, score[j].score);
	}
	Sleep(5000);											/*5초 유지*/
	
	fclose(fp);												/*파일 닫기*/
}
