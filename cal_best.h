#ifndef INCLUDED_CAL_BEST
#define INCLUDED_CAL_BEST
//------------------------------------------------
#include "hash_table.h"
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

DataItem **calculate_best(DataItem **graph_table, DataItem **inv_graph_table, DataItem **condition_table, DataItem **edge_num_table);

//------------------------------------------------
#endif