#include "cal_best.h"
#include "hash_table.h"
#include "make_graph.h"
#include "game.h"

#include <stdio.h>
#include <stdlib.h>

unsigned int next_condition_table[SIZE][3];


void calculate_best(DataItem **dictionary, unsigned int inv_dictionary[], unsigned int graph_table[SIZE][DATA_LENGTH],
                    unsigned int inv_graph_table[SIZE][DATA_LENGTH], unsigned int condition_array[SIZE], 
                    unsigned int edge_num_array[SIZE], unsigned int best_table[SIZE]){

    int *fixed_index_array = calloc(SIZE,sizeof(int));
    int *new_fixed_index_array = calloc(SIZE,sizeof(int));
    int *max_edges_to_end_table = calloc(SIZE,sizeof(int));

    

  

    for (int i = 0; i < SIZE; i++){
        for (int j = 0 ; j < 3; j++){
            next_condition_table[i][j] = 0;
        }
    }
    

    //new_fixed_index_array(初期状態)　の作成（勝敗が確定しているものをぶち込む）
    for (int i = 0; i < SIZE; i += 1){//-1で初期化
        new_fixed_index_array[i] = -1;
        fixed_index_array[i] = -1;
    }
    int c = 0;
    for (int i = 0; i < SIZE; i += 1){
        if (condition_array[i] != 1){
            
            new_fixed_index_array[c] = i;
            c += 1;
        }
    }
 


    int now_index = 0;
    unsigned int now_id = 0;
    int next_index = 0;
    unsigned int next_id = 0;
    unsigned int uncertain_id = 0;
    int uncertain_index = 0;
    int fixed_index = 0;
    int layer = 1;

    int count = 0;
    
    while(new_fixed_index_array[0] != -1){//new_fixed_index_arrayが空になるまでループ
        //new_fixed_index_arrayの中身をfixed_index_arrayに移す
        for (int i = 0; i < SIZE; i++){
            fixed_index_array[i] = new_fixed_index_array[i];
            count += 1;
            if (new_fixed_index_array[i] == -1){
                break;
            }
        }
        for (int i = 0; i < SIZE; i++){//to_check_tableを-1で初期化
            new_fixed_index_array[i] = -1;
        }
        printf("%d\n", count);
        //fixed_index_arrayの中身について、その1個前のマスの勝敗が確定するかをチェック
        int end_index = 0;
        for (int i = 0; i < SIZE; i++){
            if (fixed_index_array[i] == -1){
                break;
            }

            now_index = fixed_index_array[i];//fixed_index_arrayの中身

            for (int j = 0; j < DATA_LENGTH; j++){
                uncertain_id = inv_graph_table[now_index][j];
                if (uncertain_id == 0){
                    break;
                }
                uncertain_index = hash_search(dictionary,uncertain_id);//fixed_index_arrayの中身に遷移出来る状態
                if (condition_array[uncertain_index] == DRAW){//その状態の勝敗が未確定（引き分け）だったならこれから勝敗が決まるかをチェック
                    //next_condition_array[uncertain_index][0]:uncertain_indexから遷移できる状態のうち相手から見て負けの状態の数
                    //next_condition_array[uncertain_index][1]:uncertain_indexから遷移できる状態のうち相手から見て引き分けの状態の数
                    //next_condition_array[uncertain_index][2]:uncertain_indexから遷移できる状態のうち相手から見て勝ちの状態の数

                    //now_indexはすでに勝敗が決まっていて、uncertain_indexからも遷移できる状態なので
                    next_condition_table[uncertain_index][condition_array[now_index]] += 1;
                    if (next_condition_table[uncertain_index][LOSE] > 0){//遷移先に相手の負け状態が1つでもあれば勝ち
                        fixed_index = uncertain_index;

                        condition_array[fixed_index] = WIN;
                        new_fixed_index_array[end_index] = fixed_index;//新たに勝敗が確定したのでこの状態にたどり着く状態をこれからチェック
                        end_index += 1;
                        max_edges_to_end_table[fixed_index] = layer;
                    }
                    else if (next_condition_table[uncertain_index][WIN] == edge_num_array[uncertain_index]){
                        //遷移先のすべてが相手の勝ちなら負け
                        fixed_index = uncertain_index;

                        condition_array[fixed_index] = LOSE;
                        new_fixed_index_array[end_index] = fixed_index;
                        end_index += 1;
                        max_edges_to_end_table[fixed_index] = -layer;
                    }
                }
            }
        }
        layer += 1;
    }


    int best_id = 0;
    int tmp_max_transition_to_end = 0;
    

    //indexを走査することで全状態を走査
    for (int i = 0; i < STATE_NUM; i += 1){
        now_id = inv_dictionary[i];
        now_index = hash_search(dictionary,now_id);

        /*勝ち盤面なら遷移先の（相手目線の）負け盤面で決着手数の短いものに遷移*/
        if (condition_array[now_index] == WIN){
            best_id = 0;
            tmp_max_transition_to_end = SIZE;
            //遷移先の状態を確認
            for (int j = 0; j < DATA_LENGTH; j++){
                next_id = graph_table[now_index][j];//next_id:遷移先
                if (next_id == 0){
                    break;
                }
                next_index = hash_search(dictionary,next_id);
                if (condition_array[next_index] == LOSE){//遷移先が相手視点で負けなら・・・
                    if (max_edges_to_end_table[next_index] < tmp_max_transition_to_end){
                        tmp_max_transition_to_end = max_edges_to_end_table[next_index];
                        best_id = next_id;
                    }
                }   
            }
            best_table[now_index] = best_id;
        }
        /*負け盤面なら遷移先の（相手目線の）負け盤面で決着手数の短いものに遷移*/
        else if (condition_array[now_index] == LOSE){
            best_id = 0;
            tmp_max_transition_to_end = SIZE;

            for (int j = 0; j < DATA_LENGTH; j ++){
                next_id = graph_table[now_index][j];
                if (next_id == 0){
                    break;
                }
                next_index = hash_search(dictionary,next_id);
                if (condition_array[next_index] == WIN){
                    if (max_edges_to_end_table[next_index] < tmp_max_transition_to_end){
                        tmp_max_transition_to_end = max_edges_to_end_table[next_index];
                        best_id = next_id;
                    }
                } else{//負け盤面なら遷移先全てが相手の勝ち盤面のはず
                    printf("err\n");
                }  
            }
            best_table[now_index] = best_id;
        }
        else{
            best_id = 0;
            tmp_max_transition_to_end = SIZE;

            for (int j = 0; j < DATA_LENGTH; j++){
                next_id = graph_table[now_index][j];
                if (next_id == 0){
                    break;
                }
                next_index = hash_search(dictionary,next_id);
                if (condition_array[next_index] == 1){//引き分け盤面なら適当に遷移
                    tmp_max_transition_to_end = max_edges_to_end_table[next_index];
                    best_id = next_id;  
                    break;
                }  
            }
            best_table[now_index] = best_id;
        }
    }
}


void convert_best_array(DataItem **dict,unsigned int best_array[], short int best_move_array[]){
    int current_id,best_id;
    int current_board[5][5], best_board[5][5];
    short int start_int, end_int, move_int;
    for(int index = 0; index < SIZE; index++){
        start_int = 0;
        end_int = 0;
        current_id = hash_search(dict,index);
        best_id = best_array[index];
        
        decode_state_id(current_id,current_board);
        decode_state_id(best_id, best_board);

        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                //手の始点をさがす
                if(best_board[i][j] == EMPTY && (current_board[i][j] != best_board[i][j])){
                    start_int = (5-i) * 10 + j;
                }

                //手の終点をさがす
                if(current_board[i][j] == EMPTY && (current_board[i][j] != best_board[i][j])){
                    end_int = (5-i) * 10 + j;
                }
            }
        }

        move_int = start_int * 100 + end_int;

        best_move_array[index] = move_int;
        
    }
}




