#ifndef INCLUDED_MAKE_GRAPH
#define INCLUDED_MAKE_GRAPH
//------------------------------------------------
#include "hash_table.h"
#include "main.h"
//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------
#define STATE_NUM 3540516

//------------------------------------------------
//  型定義(Type definition)
//------------------------------------------------


//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------
void generate_board_from_array(int board[5][5], int **board_num_array);

int encode_board(int board[][5]);

void decode_board_id(int board_id, int board[][5]);

int relative_move(int board[5][5], Point cur, Vector move_vec, int turn);


int* next_board_ids(int board_id, int turn);


void make_graph(DataItem **black_table, DataItem **inv_black_table, DataItem **white_table, DataItem **inv_white_table, DataItem **condition_table);

void save_table(DataItem **table, char *file_name);
//------------------------------------------------
#endif