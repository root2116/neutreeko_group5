#ifndef INCLUDED_MAKE_GRAPH
#define INCLUDED_MAKE_GRAPH
//------------------------------------------------
#include "hash_table.h"
#include "game.h"
#include <stdio.h>
//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------
#define STATE_NUM 7081032
#define END 0
#define BLACK_INIT_STATE_ID 679486132
#define WHITE_INIT_STATE_ID 2826969780
#define DICT_PATH "../data/dict.dat"
#define INV_DICT_PATH "../data/inv_dict.dat"
#define GRAPH_TABLE_PATH "../data/graph_table.dat"
#define INV_GRAPH_TABLE_PATH "../data/inv_graph_table.dat"
#define CONDITION_ARRAY_PATH "../data/condition_array.dat"
#define BEST_ARRAY "../data/best_array.dat"

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

void make_dictionary(DataItem **dictionary, unsigned int inv_dictionary[SIZE]);

void make_graph(DataItem **dictionary, unsigned int inv_dictionary[SIZE],unsigned int graph_table[][DATA_LENGTH], unsigned int inv_graph_table[][DATA_LENGTH], unsigned int condition_array[]);

void save_hash_table(DataItem **table,char* file_path);

void recursive_save(DataItem *data_item, FILE *fpw);

void save_int_table(unsigned int table[][DATA_LENGTH], char *file_path);

void save_int_array(unsigned int array[], char* file_path);

void load_hash_table_from_file(DataItem **table, char *file_path);

void load_int_table_from_file(unsigned int table[][DATA_LENGTH], char* file_path);

void load_int_array_from_file(unsigned int array[], char* file_path);

void edge_num_count(DataItem **dict, unsigned int inv_graph_table[][DATA_LENGTH], unsigned int edge_num_array[]);

void generate_and_save_set(DataItem **dict, unsigned int inv_dict[], unsigned int graph_table[][DATA_LENGTH], unsigned int inv_graph_table[][DATA_LENGTH], unsigned int condition_array[]);

void load_set(DataItem **dict, unsigned int inv_dict[], unsigned int graph_table[][DATA_LENGTH], unsigned int inv_graph_table[][DATA_LENGTH], unsigned int condition_array[]);

void reset(unsigned int graph_table[SIZE][DATA_LENGTH]);

//------------------------------------------------
#endif