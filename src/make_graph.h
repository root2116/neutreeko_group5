#ifndef INCLUDED_MAKE_GRAPH
#define INCLUDED_MAKE_GRAPH
//------------------------------------------------
#include "hash_table.h"
#include "game.h"
#include <stdio.h>
//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------





//------------------------------------------------
//  型定義(Type definition)
//------------------------------------------------

//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------


unsigned int relative_move(int board[5][5], Point cur, Vector move_vec, int turn);

unsigned int* generate_next_state_ids(unsigned int state_id);

void make_dictionary(DataItem **dictionary, unsigned int inv_dictionary[SIZE]);

void make_graph(DataItem **dictionary, unsigned int inv_dictionary[SIZE],unsigned int graph_table[][DATA_LENGTH], unsigned int inv_graph_table[][DATA_LENGTH], unsigned int condition_array[]);

void edge_num_count(DataItem **dict, unsigned int inv_graph_table[][DATA_LENGTH], unsigned int edge_num_array[]);

void generate_and_save_set(DataItem **dict, unsigned int inv_dict[], unsigned int graph_table[][DATA_LENGTH], unsigned int inv_graph_table[][DATA_LENGTH], unsigned int condition_array[]);

void load_set(DataItem **dict, unsigned int inv_dict[], unsigned int graph_table[][DATA_LENGTH], unsigned int inv_graph_table[][DATA_LENGTH], unsigned int condition_array[]);

//------------------------------------------------
#endif