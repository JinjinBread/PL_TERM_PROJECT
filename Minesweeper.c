#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15

#define UP  0x48
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d

#define ESC 0x1b

#define SPACEBAR 32 // 아스키 코드

#define WIDTH 124
#define HEIGHT 46

#define MAX_R 99
#define MAX_C 99

#define MINE "※"
#define ZERO "　"
#define ONE "①"
#define TWO "②"
#define THREE "③"
#define FOUR "④"
#define FIVE "⑤"
#define SIX "⑥"
#define SEVEN "⑦"
#define EIGHT "⑧"
#define FLAG "▶"
#define BLOCK "■"

typedef struct mine_map {
	int mine; // 0이면 지뢰가 아님, 1이면 지뢰임.
	int mark; // 0이면 오픈 X, 1이면 오픈 O
	int flag; // 0이면 flag X, 1이면 flag O
	int mineCnt; // 내 주변 3x3의 지뢰 개수
}mine_map;

int BOARDW = 9; // 너비, 열
int BOARDH = 9; // 높이, 행
int delay = 100;
int gamelevel = 1;
int mark = 0; // 열었는지 안 열었는지 확인
int mine_num = 10;
mine_map map[MAX_R][MAX_C] = { 0 };
//mine_map map2[MAX_R][MAX_C] = { 0 };

void removeCursor(void) {

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void appearCursor(void) {

	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void gotoxy(int x, int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int fg_color, int bg_color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

void cls(int bg_color, int text_color)
{
	char cmd[100];
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);

}

void draw_box(int x1, int y1, int x2, int y2, char* ch)
{
	int x, y;
	for (x = x1; x <= x2; x += 2) {
		gotoxy(x, y1);
		printf("%s", ch);
		gotoxy(x, y2);
		printf("%s", ch);
	}
	for (y = y1; y <= y2; y++) {
		gotoxy(x1, y);
		printf("%s", ch);
		gotoxy(x2, y);
		printf("%s", ch);
	}
}

void start_menu(int x1, int y1, int x2, int y2, char* ch) {

	//removeCursor();
	system("mode con cols=124 lines=46");

	int menu_map1[7][25] = { // MINE
		1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1,
		1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
		1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
		1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1
	};

	int menu_map2[7][36] = { // sweeper
		1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 
		0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1,
		0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1
	};

	int x, y;
	
	cls(BLACK, WHITE);

	draw_box(0, 0, 122, 44, "□");

	int a1 = 36, b1 = 25;
	int a2 = 86, b2 = 39;
	textcolor(YELLOW2, BLACK);
	gotoxy(a1, b1);
	for (x = a1; x <= a2; x += 2) {
		gotoxy(x, b1);
		printf("%s", "━");
		gotoxy(x, b2);
		printf("%s", "━");
	}
	for (y = b1; y <= b2; y++) {
		gotoxy(a1, y);
		printf("%s", "┃");
		gotoxy(a2, y);
		printf("%s", "┃");
	}
	gotoxy(a1, b1); printf("┏");
	gotoxy(a2, b1); printf("┓");
	gotoxy(a1, b2); printf("┗");
	gotoxy(a2, b2); printf("┛");

	gotoxy(55, 29);
	printf("1. 게임 시작");
	gotoxy(55, 32);
	printf("2. 난이도 선택");
	gotoxy(55, 35);
	printf("0. 종료");

	int bg_color, fg_color;

	while (1) {
		do {
			bg_color = rand() % 16;
			fg_color = rand() % 16;
		} while (fg_color == WHITE || bg_color == BLACK);

		for (x = 0; x < 7; x++) {
			gotoxy(x1, y1 + x);
			for (y = 0; y < 25; y++) {
				if (menu_map1[x][y] == 1) {
					textcolor(fg_color, bg_color);
					printf("%s", "■");
				}
				else {
					textcolor(BLACK, BLACK);
					printf("  ");
				}
			}
		}

		do {
			bg_color = rand() % 16;
			fg_color = rand() % 16;
		} while (fg_color == WHITE || bg_color == BLACK || fg_color == RED2);

		for (x = 0; x < 7; x++) {
			gotoxy(x2, y2 + x);
			for (y = 0; y < 36; y++) {
				if (menu_map2[x][y] == 1) {
					if ((x == 6 && (y == 22 || y == 24 || y == 25)) || (y == 23) || ((x == 1 || x == 2 || x == 3) && y == 24) || ((x == 2 || x == 6) && y == 25)) // P shaped flag
						textcolor(RED2, bg_color);
					else textcolor(fg_color, bg_color);
					printf("%s", "■");
				}
				else {
					textcolor(BLACK, BLACK);
					printf("  ");
				}
			}
		}

		Sleep(200);

		textcolor(YELLOW2, BLACK);
		gotoxy(50, 40);

		if (kbhit()) {
			(*ch) = getch();
			if ((*ch) == '1' || (*ch) == '2' || (*ch) == '0')
				break;
		}
	}
}

void rand_map() { // board 크기만큼 0 or 1로 랜덤하게 맵 생성
	srand(time(NULL));
	int prob;
	int minenum;
	int x, y;
	if (gamelevel == 1) { // 초급 지뢰 10개
		minenum = mine_num;
		while (minenum--) {
			do {
				x = rand() % BOARDH; // 행
				y = rand() % BOARDW; // 열
			} while (map[x][y].mine == 1);
			map[x][y].mine = 1;
		}
	}
	else if (gamelevel == 2) { // 중급 지뢰 40개
		minenum = mine_num;
		while (minenum--) {
			do {
				x = rand() % BOARDH;
				y = rand() % BOARDW;
			} while (map[x][y].mine == 1);
			map[x][y].mine = 1;
		}
	}
	else { // 고급 지뢰 99개
		minenum = mine_num;
		while (minenum--) {
			do {
				x = rand() % BOARDH;
				y = rand() % BOARDW;
			} while (map[x][y].mine == 1);
			map[x][y].mine = 1;
		}
	}
}

void create_map() { // 3 x 3 폭탄 수 세기
	int x, y;
	rand_map();
	/*
	1 2 3
	4 □ 5
	6 7 8
	*/
	for (x = 0; x < BOARDH; x++) { // y좌표 (행)
		for (y = 0; y < BOARDW; y++) { // x좌표 (열)
			map[x][y].mineCnt = 0;
			if (map[x][y].mine == 0) {
				if ((x - 1) >= 0) { // y좌표가 벗어나지 않게
					if ((y - 1) >= 0 && map[x - 1][y - 1].mine == 1) map[x][y].mineCnt++; // 1
					if (map[x - 1][y].mine == 1) map[x][y].mineCnt++; // 2
					if ((y + 1) <= BOARDW - 1 && map[x - 1][y + 1].mine == 1) map[x][y].mineCnt++; // 3
				}
				if ((y - 1) >= 0 && map[x][y - 1].mine == 1) map[x][y].mineCnt++; // 4
				if ((y + 1) <= BOARDW - 1 && map[x][y + 1].mine == 1) map[x][y].mineCnt++; // 5

				if ((x + 1) <= (BOARDH - 1)) {
					if ((y - 1) >= 0 && map[x + 1][y - 1].mine == 1) map[x][y].mineCnt++; // 6
					if (map[x + 1][y].mine == 1) map[x][y].mineCnt++; // 7
					if ((y + 1) <= BOARDW - 1 && map[x + 1][y + 1].mine == 1) map[x][y].mineCnt++; // 8
				}
			}
		}
	}
}

void show_map(int minenum) {
	int i, j;
	if (gamelevel == 1) {
		gotoxy(32, 5);
		textcolor(YELLOW2, BLACK);
		printf("깃발의 수 : %2d", minenum);
		for (i = 0; i < BOARDH; i++) { // 9 x 9 board // y = 0, y = 10 구간에 벽생성
			textcolor(CYAN1, BLACK);
			draw_box(0, 0, 20, 10, "■"); // 테두리
			gotoxy(2, 1 + i);
			for (j = 0; j < BOARDW; j++) {
				textcolor(WHITE, BLACK);
				if (map[i][j].mark == 0) { // 오픈 안 했으면
					if (map[i][j].flag == 0)
						printf("%s", BLOCK);
					else
						printf("%s", FLAG);
				}
				else {
					if (map[i][j].mine != 1) { // 오픈 했는데, 지뢰가 아니면
						if (map[i][j].mineCnt == 0) printf("%s", ZERO);
						else if (map[i][j].mineCnt == 1) printf("%s", ONE);
						else if (map[i][j].mineCnt == 2) printf("%s", TWO);
						else if (map[i][j].mineCnt == 3) printf("%s", THREE);
						else if (map[i][j].mineCnt == 4) printf("%s", FOUR);
						else if (map[i][j].mineCnt == 5) printf("%s", FIVE);
						else if (map[i][j].mineCnt == 6) printf("%s", SIX);
						else if (map[i][j].mineCnt == 7) printf("%s", SEVEN);
						else if (map[i][j].mineCnt == 8) printf("%s", EIGHT);
					}
					else {
						textcolor(RED2, BLACK);
						printf("%s", MINE);
					}
				}
			}
			printf("\n");
		}
	}
	else if (gamelevel == 2) {
		gotoxy(44, 6);
		textcolor(YELLOW2, BLACK);
		printf("깃발의 수 : %2d", minenum);
		for (i = 0; i < BOARDH; i++) { // 16 x 16 board // y = 0, y = 17 구간에 벽생성
			textcolor(CYAN1, BLACK);
			draw_box(0, 0, 34, 17, "■");
			gotoxy(2, 1 + i);
			for (j = 0; j < BOARDW; j++) {
				textcolor(WHITE, BLACK);
				if (map[i][j].mark == 0) {
					if (map[i][j].flag == 0)
						printf("%s", BLOCK);
					else
						printf("%s", FLAG);
				}
				else {
					if (map[i][j].mine != 1) {
						if (map[i][j].mineCnt == 0) printf("%s", ZERO);
						else if (map[i][j].mineCnt == 1) printf("%s", ONE);
						else if (map[i][j].mineCnt == 2) printf("%s", TWO);
						else if (map[i][j].mineCnt == 3) printf("%s", THREE);
						else if (map[i][j].mineCnt == 4) printf("%s", FOUR);
						else if (map[i][j].mineCnt == 5) printf("%s", FIVE);
						else if (map[i][j].mineCnt == 6) printf("%s", SIX);
						else if (map[i][j].mineCnt == 7) printf("%s", SEVEN);
						else if (map[i][j].mineCnt == 8) printf("%s", EIGHT);
					}
					else {
						textcolor(RED2, BLACK);
						printf("%s", MINE);
					}
				}
			}
			printf("\n");
		}
	}
	else {
		gotoxy(44, 9);
		textcolor(YELLOW2, BLACK);
		printf("깃발의 수 : %2d", minenum);
		for (i = 0; i < BOARDH; i++) { // 16 x 30 board // y = 0, y = 31 구간에 벽생성
			textcolor(CYAN1, BLACK);
			draw_box(0, 0, 34, 31, "■");
			gotoxy(2, 1 + i);
			for (j = 0; j < BOARDW; j++) {
				textcolor(WHITE, BLACK);
				if (map[i][j].mark == 0) {
					if (map[i][j].flag == 0)
						printf("%s", BLOCK);
					else
						printf("%s", FLAG);
				}
				else {
					if (map[i][j].mine != 1) {
						if (map[i][j].mineCnt == 0) printf("%s", ZERO);
						else if (map[i][j].mineCnt == 1) printf("%s", ONE);
						else if (map[i][j].mineCnt == 2) printf("%s", TWO);
						else if (map[i][j].mineCnt == 3) printf("%s", THREE);
						else if (map[i][j].mineCnt == 4) printf("%s", FOUR);
						else if (map[i][j].mineCnt == 5) printf("%s", FIVE);
						else if (map[i][j].mineCnt == 6) printf("%s", SIX);
						else if (map[i][j].mineCnt == 7) printf("%s", SEVEN);
						else if (map[i][j].mineCnt == 8) printf("%s", EIGHT);
					}
					else {
						textcolor(RED2, BLACK);
						printf("%s", MINE);
					}
				}
			}
			printf("\n");
		}
	}
}

void square(int x1, int y1, int x2, int y2) {
	int x, y;

	for (x = x1; x <= x2; x += 2) {
		gotoxy(x, y1);
		printf("━");
		gotoxy(x, y2);
		printf("━");
	}

	for (y = y1; y <= y2; y++) {
		gotoxy(x1, y);
		printf("┃");
		gotoxy(x2, y);
		printf("┃");
	}

	gotoxy(x1, y1); printf("┏");
	gotoxy(x2, y1); printf("┓");
	gotoxy(x1, y2); printf("┗");
	gotoxy(x2, y2); printf("┛");
}

void show_info() { // 방향키로 움직일 수 있고, 스페이스바는 선택, F or f는 깃발 꽂을 수 있음을 알려줌.
	
	gotoxy(71, 11);
	printf("이동");

	square(88, 9, 92, 11);
	gotoxy(90, 10);
	printf("↑");
	square(82, 12, 86, 14);
	gotoxy(84, 13);
	printf("←");
	square(94, 12, 98, 14);
	gotoxy(96, 13);
	printf("→");
	square(88, 12, 92, 14);
	gotoxy(90, 13);
	printf("↓");

	gotoxy(71, 21);
	printf("선택");

	square(82, 20, 98, 22);
	gotoxy(86, 21);
	printf("ＳＰＡＣＥ");


	gotoxy(71, 29);
	printf("깃발");

	square(82, 28, 86, 30);
	gotoxy(84, 29);
	printf("Ｆ");

	gotoxy(90, 29);
	printf("or");

	square(94, 28, 98, 30);
	gotoxy(96, 29);
	printf("ｆ");
}

void show_level_map(int x1, int y1, int x2, int y2) {
	cls(BLACK, WHITE);
	textcolor(YELLOW2, BLACK);
	int x, y;
	gotoxy(x1, y1);
	for (x = x1; x <= x2; x += 2) {
		gotoxy(x, y1);
		printf("%s", "━");
		gotoxy(x, y2);
		printf("%s", "━");
	}
	for (y = y1; y <= y2; y++) {
		gotoxy(x1, y);
		printf("%s", "┃");
		gotoxy(x2, y);
		printf("%s", "┃");
	}
	gotoxy(x1, y1); printf("┏");
	gotoxy(x2, y1); printf("┓");
	gotoxy(x1, y2); printf("┗");
	gotoxy(x2, y2); printf("┛");

	gotoxy(54, 14);
	printf(" ※ 레벨 선택 ※ ");
	gotoxy(47, 18);
	printf("1.　초　　　　급　　( ９ Ｘ ９ )");
	gotoxy(47, 22);
	printf("2.　중　　　　급　　( １６ Ｘ １６ )");
	gotoxy(47, 26);
	printf("3.　고　　　　급　　( １６ Ｘ ３０ )");
}

void choose_level() {
	
	// 초급이면 BOARDW와 BOARDH를 9 x 9
	// 중급이면 BOARDW와 BOARDH를 16 x 16
	// 고급이면 BOARDW와 BOARDH를 16 x 30
	char ch;
	show_level_map(36, 12, 86, 29);
	removeCursor();
	while (1) {
		ch = getch();

		if (ch == '1') {
			gamelevel = 1;
			BOARDW = 9;
			BOARDH = 9;
			mine_num = 10;
			break;
		}
		else if (ch == '2') {
			gamelevel = 2;
			BOARDW = 16;
			BOARDH = 16;
			mine_num = 40;
			break;
		}
		else if (ch == '3') {
			gamelevel = 3;
			BOARDW = 16;
			BOARDH = 30;
			mine_num = 99;
			break;
		}
		else if (ch == ESC)
			break;
	}
	appearCursor();
}

void show_time(int run_time) {
	if (gamelevel == 1) {
		gotoxy(32, 4);
		textcolor(YELLOW2, BLACK);
		printf("경과 시간 : %2d", run_time);
	}
	else if (gamelevel == 2) {
		gotoxy(44, 5);
		textcolor(YELLOW2, BLACK);
		printf("경과 시간 : %2d", run_time);
	}
	else {
		gotoxy(44, 8);
		textcolor(YELLOW2, BLACK);
		printf("경과 시간 : %2d", run_time);
	}
	textcolor(WHITE, BLACK);
}

void CheckMine(int x, int y)
{
	int i, j;
	if (x < 0 || y < 0 || x > BOARDH - 1 || y > BOARDW - 1) return; // BOARD를 벗어나면 return
	if (map[x][y].mark == 1) return;
	//if (map[x][y].mine == 1) return;
	if (map[x][y].mineCnt > 0) { // ZERO가 아닌 것을 눌렀을 때,
		map[x][y].mark = 1;
		return;
	}

	map[x][y].mark = 1;
	for (i = x - 1; i < x + 2; i++) { // 대각선, 좌우를 확인
		for (j = y - 1; j < y + 2; j++) {
			CheckMine(i, j);
		}
	}
}

void init_game() { // map reset
	//player.X = 2; player.Y = 1;
	for (int i = 0; i < MAX_R; i++)
		for (int j = 0; j < MAX_C; j++) {
			map[i][j].mark = 0;
			map[i][j].mine = 0;
			map[i][j].flag = 0;
			map[i][j].mineCnt = 0;
		}
}

int is_succeed(int flag_num) { // 열리지 않은 블럭의 수 = 지뢰의 수 + flag의 수가 0
	int i, j, mark_num = 0;
	for (i = 0; i < BOARDW; i++) {
		for (j = 0; j < BOARDH; j++) {
			if (map[i][j].mark == 0)
				mark_num++;
		}
	}
	if ((mark_num == mine_num) && flag_num == 0)
		return 1;
	return 0;
}

void start_game() { // minenum == flag 개수
	int run_time = 0, elapsed_time = 0;
	int minenum = mine_num;
	char ch;
	COORD player;
	clock_t start, end;
	start = clock();
	create_map();
	cls(BLACK, WHITE);
	show_map(minenum);
	player.X = 2; player.Y = 1;
	gotoxy(player.X, player.Y); // 재시작 할려면 초기화 해줘야함.
	
	show_info();
	show_time(run_time);

	while(1) {
		end = clock();
		run_time = (end - start) / CLOCKS_PER_SEC;
		if (run_time > elapsed_time) {
			show_time(run_time);
			elapsed_time = run_time;
		}

		gotoxy(player.X, player.Y);

		if (kbhit()) {
			ch = getch();
			if (ch == ESC)
				break;
			switch (ch) {
			case UP:
				if (player.Y > 1) {
					player.Y = player.Y - 1;
					gotoxy(player.X, player.Y);
				}
				break;
			case DOWN:
				if (player.Y < BOARDH) {
					player.Y = player.Y + 1;
					gotoxy(player.X, player.Y);
				}
				break;
			case LEFT:
				if (player.X > 2) {
					player.X = player.X - 2;
					gotoxy(player.X, player.Y);
				}
				break;
			case RIGHT:
				if (player.X < BOARDW * 2) {
					player.X = player.X + 2;
					gotoxy(player.X, player.Y);
				}
				break;

			// 스페이스바는 선택, F or f는 FLAG
			case SPACEBAR:
				if (map[player.Y - 1][(player.X - 2) / 2].mine != 1) { // 지뢰가 아니라면
					CheckMine(player.Y - 1, (player.X - 2) / 2); // 이차원 배열에서는 x와 y가 반대
					if (is_succeed(minenum) == 1) {
						show_map(minenum);
						if (gamelevel == 1) {
							gotoxy(32, 7);
							textcolor(YELLOW2, BLACK);
							printf("Good!!");
							getch(ch);
							return;
						}
						else if (gamelevel == 2) {
							gotoxy(44, 8);
							textcolor(YELLOW2, BLACK);
							printf("Good!!");
							getch(ch);
							return;
						}
						else {
							gotoxy(44, 11);
							textcolor(YELLOW2, BLACK);
							printf("Good!!");
							getch(ch);
							return;
						}
					}
				}
				else { // 지뢰라면
					map[player.Y - 1][(player.X - 2) / 2].mark = 1;
					show_map(minenum);
					if (gamelevel == 1) {
						gotoxy(32, 7);
						textcolor(RED2, BLACK);
						printf("You Die..");
						getch(ch);
						return;
					}
					else if (gamelevel == 2) {
						gotoxy(44, 8);
						textcolor(RED2, BLACK);
						printf("You Die..");
						getch(ch);
						return;
					}
					else {
						gotoxy(44, 11);
						textcolor(RED2, BLACK);
						printf("You Die..");
						getch(ch);
						return;
					}
				}
				show_map(minenum);
				break;
			case 'F':
			case 'f':
				if (map[player.Y - 1][(player.X - 2) / 2].mark == 1) // 오픈된 곳은 flag 못 꽂음.
					map[player.Y - 1][(player.X - 2) / 2].flag = 0;
				if (map[player.Y - 1][(player.X - 2) / 2].flag == 0) {
					map[player.Y - 1][(player.X - 2) / 2].flag = 1;
					minenum--;
				}
				else {
					map[player.Y - 1][(player.X - 2) / 2].flag = 0;
					minenum++;
				}
				if (is_succeed(minenum) == 1) {
					show_map(minenum);
					if (gamelevel == 1) {
						gotoxy(32, 7);
						textcolor(YELLOW2, BLACK);
						printf("Good!!");
						getch(ch);
						return;
					}
					else if (gamelevel == 2) {
						gotoxy(44, 8);
						textcolor(YELLOW2, BLACK);
						printf("Good!!");
						getch(ch);
						return;
					}
					else {
						gotoxy(44, 11);
						textcolor(YELLOW2, BLACK);
						printf("Good!!");
						getch(ch);
						return;
					}
				}
				show_map(minenum);
				break;
			}
			gotoxy(player.X, player.Y);
		}
		Sleep(10);
	}
}

int main() {
	char ch;

	while (1) {
		init_game();
		start_menu(10, 5, 42, 14, &ch);
		switch (ch) {
		case '1':
			start_game();
			break;
		case '2':
			choose_level();
			break;
		case '0':
			gotoxy(43, 40);
			exit(1);
		}
	}
	gotoxy(27, 31);
	return 0;
}