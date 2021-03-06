#ifndef INCLUDED_GAME
#define INCLUDED_GAME
//------------------------------------------------
#include "hash_table.h"
//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------
#define EMPTY 0
#define BLACK 1
#define WHITE 2

//------------------------------------------------
//  型定義(Type definition)
//------------------------------------------------

typedef struct
{
    int x;
    int y;
} Vector;

typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    Point start;
    Point end;
} Move;

//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------
int gcd(int x, int y);

int ctoi(char c);

void parser(char *input, Move *move);

void make_vec(Point *a, Point *b, Vector *v);

void add_vec_to_point(Point *a, Vector *v, Point *b);

int judge_one_side(int board[5][5], int color);

int judge(int board[5][5]);

int is_on_wall(Point p);

int can_move(int board[5][5], Move move, int turn);

int move_piece(int board[5][5], Move move, int turn);

void update_turn(int *turn, int *turn_count);

void display_board(int board[5][5]);

void start_game(int init_turn, DataItem **dict,short int best_move_array[]);

void game_ai(DataItem **dict,short int best_move_array[], int board[][5], int turn, char* best_move);

void convert_move_int_into_move_string(short int move_int, char move[4]);
//------------------------------------------------
#endif