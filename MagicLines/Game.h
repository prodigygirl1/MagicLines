#pragma once
#include "framework.h"
#define MAX_SIZE 7
#define BALLS_FIRST 3
#define SIZE_SQUARE 50
#define COLORS_NUM 5
#define IS_OCCUPIED 65500
extern struct point_t;
int random_num(int n1, int n2);
void shuffle_values(point_t* coords, int size);
int add_balls(int number_balls, struct point_t* new_balls);
void init_board(int number_balls);
void board_paint(HDC hdc, int square_size);
bool point_in_range(int i, int j);
bool wave_algorithm(int x1, int y1, int x2, int y2, int d, int dublicat_board[MAX_SIZE][MAX_SIZE]);
bool check_path(int x1, int y1, int x2, int y2);
void move_ball(int x1, int y1, int x2, int y2);
bool delete_balls(int x, int y);
void test_delete_balls();
bool board_is_full();
int loadLevel();
int saveLevel();
void new_game();
