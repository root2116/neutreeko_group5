
#include "hash_table.h"
#include "make_graph.h"
#include "cal_best.h"
#include "game.h"

#include<stdio.h>
#include<stdlib.h>



unsigned int graph_table[SIZE][DATA_LENGTH];
unsigned int inv_graph_table[SIZE][DATA_LENGTH];
unsigned int condition_array[SIZE];
unsigned int edge_num_array[SIZE];
unsigned int best_array[SIZE];
DataItem *dict[SIZE];
unsigned int inv_dict[SIZE];

int main(int argc,char *argv[]){

    

    //テスト----------------------------------------------------------------

    

    generate_and_save_set(dict,inv_dict,graph_table,inv_graph_table,condition_array);
    // reconsturct_set(dict,inv_dict,graph_table,inv_graph_table,condition_array);

    
    //edge_num_table
    
    // edge_num_count(dict, inv_graph_table,edge_num_array);
    // printf("edge_num_count finished\n");


    //cal_best
    // calculate_best(dict,inv_dict,graph_table,inv_graph_table,condition_array,edge_num_array,best_array);
    // printf("calculate_best finished\n");

    //-----------------------------------------------------------------------

   
    
    
   



    return 0;
}
