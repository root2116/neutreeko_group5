#ifndef INCLUDED_TEST_UTIL
#define INCLUDED_TEST_UTIL
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

int is_same_board(int array1[5][5], int array2[5][5]);

int is_same_table(DataItem **table1, DataItem **table2);

int count_data_items(DataItem **table);

//------------------------------------------------
#endif