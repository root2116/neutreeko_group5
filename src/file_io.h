#ifndef INCLUDED_FILE_IO
#define INCLUDED_FILE_IO
//------------------------------------------------
#include "hash_table.h"
#include "game.h"
#include <stdio.h>
//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------


#define DICT_PATH "../data/dict.dat"
#define INV_DICT_PATH "../data/inv_dict.dat"
#define GRAPH_TABLE_PATH "../data/graph_table.dat"
#define INV_GRAPH_TABLE_PATH "../data/inv_graph_table.dat"
#define CONDITION_ARRAY_PATH "../data/condition_array.dat"
#define BEST_MOVE_ARRAY_PATH "../data/best_move_array.dat"

//------------------------------------------------
//  型定義(Type definition)
//------------------------------------------------

//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------


void save_hash_table(DataItem **table, char *file_path);

void recursive_save(DataItem *data_item, FILE *fpw);

void save_int_table(unsigned int table[][DATA_LENGTH], char *file_path);

void save_int_array(unsigned int array[], char *file_path);

void save_short_array(short int array[], char *file_path);

void load_hash_table_from_file(DataItem **table, char *file_path);

void load_int_table_from_file(unsigned int table[][DATA_LENGTH], char *file_path);

void load_int_array_from_file(unsigned int array[], char *file_path);

void load_short_array_from_file(short int array[], char *file_path);




void show_graph(DataItem **dictionary, unsigned int graph_table[SIZE][DATA_LENGTH], int c);

//------------------------------------------------
#endif