
#include "hash_table.h"
#include "make_graph.h"
#include "cal_best.h"
#include "game.h"

#include<stdio.h>
#include<stdlib.h>



unsigned int graph_table[SIZE][DATA_LENGTH];
unsigned int inv_graph_table[SIZE][DATA_LENGTH];
unsigned int condition_array[SIZE] = {};
unsigned int edge_num_array[SIZE] = {};
unsigned int best_array[SIZE] = {};
DataItem *dict[SIZE];
unsigned int inv_dict[SIZE] = {};


int main(int argc,char *argv[]){

    reset(graph_table);
    reset(inv_graph_table);

    //テスト----------------------------------------------------------------


    // generate_and_save_set(dict,inv_dict,graph_table,inv_graph_table,condition_array);
    load_set(dict,inv_dict,graph_table,inv_graph_table,condition_array);
    
    // show_graph(dict,inv_graph_table,50);
    //edge_num_table
    
    edge_num_count(dict, inv_graph_table,edge_num_array);

    // printf("%d\n",edge_num_array[0]);

    printf("edge_num_count finished\n");


    // cal_best
    calculate_best(dict,inv_dict,graph_table,inv_graph_table,condition_array,edge_num_array,best_array);
    printf("calculate_best finished\n");

    //-----------------------------------------------------------------------

   
    
    
   



    return 0;
}

