#ifndef INCLUDED_MAKE_GRAPH
#define INCLUDED_MAKE_GRAPH
//------------------------------------------------
#include "hash_table.h"
#include "game.h"

#include <stdio.h>
//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------
#define WIN 2
#define LOSE 0
#define DRAW 1
#define MAX_FIXED 6871764

//------------------------------------------------
//  型定義(Type definition)
//------------------------------------------------

//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------


unsigned int relative_move(int board[5][5], Point cur, Vector move_vec, int turn);

unsigned int* generate_next_state_ids(unsigned int state_id);

void make_dictionary();

void make_graph();

void edge_num_count(unsigned int graph_table[][DATA_LENGTH]);

void calculate_best();

void convert_best_array(short int best_move_array[]);

void generate_and_save_set();

void load_set();

void generate_and_save_best_move_array();


//------------------------------------------------
#endif