
#include "hash_table.h"
#include "game.h"
#include "file_io.h"


#include<stdio.h>
#include<stdlib.h>



int main(int argc,char *argv[]){
    // 一回目の実行でアンコメントする
    // generate_and_save_set(); 

    // 二回目の実行でアンコメントする
    // generate_and_save_best_move_array();
    
    
    short int *best_move_array = (short int*)calloc(SIZE,sizeof(short int));
   
    load_short_array_from_file(best_move_array,BEST_MOVE_ARRAY_PATH);

    DataItem *dict = (DataItem *)calloc(SIZE, sizeof(DataItem));

    load_hash_table_from_file((DataItem**)dict,DICT_PATH);

    start_game(atoi(argv[1]),(DataItem**)dict,best_move_array);

    return 0;
}

