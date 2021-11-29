#ifndef INCLUDED_CAL_BEST
#define INCLUDED_CAL_BEST
//------------------------------------------------
#include "hash_table.h"
//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------
#define WIN 2
#define LOSE 0
#define DRAW 1
//------------------------------------------------
//  型定義(Type definition)
//------------------------------------------------

//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------

void calculate_best(DataItem **dictionary, unsigned int inv_dictionary[], unsigned int graph_table[SIZE][DATA_LENGTH],
                    unsigned int inv_graph_table[SIZE][DATA_LENGTH], unsigned int condition_array[SIZE],
                    unsigned int edge_num_array[SIZE], unsigned int best_array[SIZE]);

//------------------------------------------------
#endif