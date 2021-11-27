
#include "make_graph.h"
#include "hash_table.h"
#include "main.h"


#include <stdio.h>
#include <stdlib.h>

DataItem* calculate_best(DataItem **graph_table,DataItem **inv_graph_table,DataItem **condition_table, DataItem **edge_num_table){
    DataItem *next_condition_table = malloc(sizeof(DataItem)*SIZE);
    DataItem *max_transition_end_table = malloc(sizeof(DataItem)*SIZE);
    DataItem *best_table = malloc(sizeof(DataItem)*SIZE);

    int w1,w2,w3,b1,b2,b3;
    int *board_num_array[6] = {&w1,&w2,&w3,&b1,&b2,&b3};
    int board[5][5] = {};
    int now_id = 0;
    int turn;

    for(w1 = 0; w1 < 25; w1++){
        for(w2 = w1 + 1; w2 < 25; w2++){
            for(w3 = w2 + 1; w3 < 25; w3++){
                for(b1 = 0; b1 < 25; b1++){
                    if(b1 == w1 || b1 == w2 || b1 == w3) continue;
                    for(b2 = b1 + 1; b2 < 25; b2++){
                        if(b2 == w1 || b2 == w2 || b2 == w3) continue;
                        for(b3 = b2 + 1; b3 < 25; b3++){
                            if(b3 == w1 || b3 == w2 || b3 == w3) continue;
                            for(turn = 1; turn < 3; turn += 1){
                            
                                generate_board_from_array(board,board_num_array);
                                now_id = encode_board(board, turn);
                                hash_insert(&next_condition_table, now_id, malloc(sizeof(int)*DATA_LENGTH));
                                hash_insert(&max_transition_end_table, now_id, malloc(sizeof(int)*DATA_LENGTH));
                                hash_insert(&best_table, now_id, malloc(sizeof(int)*DATA_LENGTH));
                            }
                        }
                    }
                }
            }
        }
    }



    int checked_id_table[SIZE];
    int to_check_id_table[SIZE];


    int i = 0;
    int j = 0;
    int next_id = 0;
    
    int c = 0;
    int layer = 1;
    while(to_check_id_table[0] != 0){
        for (i = 0; i < SIZE; i += 1){
            checked_id_table[i] = to_check_id_table[i];
            if (to_check_id_table[i] == 0){
                break;
            }
        }
        c = 0;
        for (i = 0; i < SIZE; i += 1){
            if (to_check_id_table[i] == 0){
                break;
            }
            now_id = to_check_id_table[i];
            for (j = 0; j < DATA_LENGTH; j += 1){
                next_id = hash_search(inv_graph_table,checked_id_table[i])[j];
                if (next_id == 0){
                    break;
                }
                
                if (hash_search(condition_table,now_id)[0] == 0){
                    hash_search(&next_condition_table,now_id)[hash_search(condition_table,now_id)[0]] += 1;
                    if (hash_search(&next_condition_table,now_id)[0] > 0){
                        hash_search(condition_table,now_id)[0] = 1;
                        to_check_id_table[c] = now_id;
                        c += 1;
                        hash_search(&max_transition_end_table,now_id)[0] = layer;
                    }
                    else if (hash_search(&next_condition_table,now_id)[2] == hash_search(edge_num_table,now_id)[0]  /*　==要素数*/){
                        hash_search(condition_table,now_id)[0] = -1;
                        to_check_id_table[c] = now_id;
                        c += 1;
                        hash_search(&max_transition_end_table,now_id)[0] = -layer;
                    }
                    
                }
            }
        }
        layer += 1;
    }


    int best_id = 0;
    int tmp_max_transition_end = 0;


    for(w1 = 0; w1 < 25; w1++){
        for(w2 = w1 + 1; w2 < 25; w2++){
            for(w3 = w2 + 1; w3 < 25; w3++){
                for(b1 = 0; b1 < 25; b1++){
                    if(b1 == w1 || b1 == w2 || b1 == w3) continue;
                    for(b2 = b1 + 1; b2 < 25; b2++){
                        if(b2 == w1 || b2 == w2 || b2 == w3) continue;
                        for(b3 = b2 + 1; b3 < 25; b3++){
                            if(b3 == w1 || b3 == w2 || b3 == w3) continue;
                            for (turn = 1; turn < 3; turn += 1){
                                generate_board_from_array(board,board_num_array);
                                now_id = encode_board(board, turn);

                                /*勝ち盤面なら遷移先の（相手目線の）負け盤面で決着手数の短いものに遷移*/
                                if (hash_search(condition_table,now_id)[0] == 1){
                                    best_id = 0;
                                    tmp_max_transition_end = -SIZE;

                                    for (j = 0; j < DATA_LENGTH; j += 1){
                                        next_id = hash_search(graph_table,now_id)[j];
                                        if (next_id == 0){
                                            break;
                                        }
                                        if (hash_search(condition_table,next_id)[0] == -1){
                                            if (hash_search(&max_transition_end_table,next_id)[0] > tmp_max_transition_end){
                                                tmp_max_transition_end = hash_search(&max_transition_end_table,next_id)[0];
                                                best_id = next_id;
                                            }
                                        }   
                                    }
                                    hash_search(&best_table,now_id)[0] = best_id;
                                }
                                /*負け盤面なら遷移先の（相手目線の）負け盤面で決着手数の短いものに遷移*/
                                else if (hash_search(condition_table,now_id)[0] == -1){
                                    best_id = 0;
                                    tmp_max_transition_end = -SIZE;

                                    for (j = 0; j < DATA_LENGTH; j += 1){
                                        next_id = hash_search(graph_table,now_id)[j];
                                        if (next_id == 0){
                                            break;
                                        }
                                        if (hash_search(condition_table,next_id)[0] == 1){
                                            if (hash_search(&max_transition_end_table,next_id)[0] > tmp_max_transition_end){
                                                tmp_max_transition_end = hash_search(&max_transition_end_table,next_id)[0];
                                                best_id = next_id;
                                            }
                                        }else{
                                        printf("err");
                                        }
                                    }
                                    hash_search(&best_table,now_id)[0] = best_id;
                                }
                                else{
                                    best_id = 0;
                                    tmp_max_transition_end = -SIZE;

                                    for (j = 0; j < DATA_LENGTH; j += 1){
                                        next_id = hash_search(graph_table,now_id)[j];
                                        if (next_id == 0){
                                            break;
                                        }
                                        if (hash_search(condition_table,next_id)[0] == 0){
                                            tmp_max_transition_end = hash_search(&max_transition_end_table,next_id)[0];
                                            best_id = next_id;
                                        }
                                    }
                                    hash_search(&best_table,now_id)[0] = best_id;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    free(next_condition_table);
    free(max_transition_end_table);
    return best_table;
}
