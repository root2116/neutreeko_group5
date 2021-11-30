#ifndef INCLUDED_UTILITY
#define INCLUDED_UTILITY
//------------------------------------------------
#include "hash_table.h"
#include "game.h"
#include <stdio.h>
//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------
#define STATE_NUM 7081032
#define END 0

//------------------------------------------------
//  型定義(Type definition)
//------------------------------------------------

//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------
void generate_board_from_array(int board[5][5], int **board_num_array);

unsigned int encode_board(int board[][5], int turn);

void decode_state_id(unsigned int board_id, int board[][5]);

int get_color_from_state_id(unsigned int state_id);

void init_table(unsigned int graph_table[SIZE][DATA_LENGTH]);

void show_graph(DataItem **dictionary, unsigned int graph_table[SIZE][DATA_LENGTH], int c);

//------------------------------------------------
#endif