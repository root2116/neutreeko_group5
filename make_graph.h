#ifndef INCLUDED_MAKE_GRAPH
#define INCLUDED_MAKE_GRAPH
//------------------------------------------------
#include "hash_table.h"
#include "main.h"
#include <stdio.h>
//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------
#define STATE_NUM 7081032
#define END 0
#define BLACK_INIT_STATE_ID 679486132
#define WHITE_INIT_STATE_ID 2826969780

//------------------------------------------------
//  型定義(Type definition)
//------------------------------------------------

//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------
void generate_board_from_array(int board[5][5], int **board_num_array);

unsigned int encode_board(int board[][5],int turn);

void decode_state_id(unsigned int board_id, int board[][5]);

unsigned int relative_move(int board[5][5], Point cur, Vector move_vec, int turn);


unsigned int* generate_next_state_ids(unsigned int state_id);

void make_graph(DataItem **graph_table, DataItem **inv_graph_table, DataItem **condition_table);

void remove_unreachable_states(DataItem **graph_table, DataItem **condition_table);

void graph_search(DataItem **seen, unsigned int init_state_id, DataItem **graph_table);

void recursive_delete(DataItem **seen, DataItem *data_item, DataItem **graph_table, DataItem **condition_table);

void save_table(DataItem **table, char *file_path);

void recursive_save(DataItem *data_item, FILE *fpw);

void reconstruct_table_from_file(DataItem **table, char *file_path);

void edge_num_count(DataItem **graph_table, DataItem **edge_num_table);

void recursive_count(DataItem **edge_num_table, DataItem *data_item);

//------------------------------------------------
#endif