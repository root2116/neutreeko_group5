#include "cal_best.h"
#include "make_graph.h"
#include "hash_table.h"
#include "main.h"


#include <stdio.h>
#include <stdlib.h>
int checked_index_table[SIZE] = {};
int to_check_index_table[SIZE] = {};
unsigned int next_condition_table[SIZE][3];
unsigned int max_edges_to_end_table[SIZE] = {};

void calculate_best(DataItem **dictionary, unsigned int inv_dictionary[], unsigned int graph_table[SIZE][DATA_LENGTH],
                    unsigned int inv_graph_table[SIZE][DATA_LENGTH], unsigned int condition_array[SIZE], 
                    unsigned int edge_num_array[SIZE], unsigned int best_table[SIZE]){
    int i = 0;
    int j = 0;
    int c = 0;

    for (i = 0; i < SIZE; i += 1){
        for (j = 0 ; j < 2; j += 1){
            next_condition_table[i][j] = 0;
        }
    }
    

    //to_check_index_table(初期状態)　の作成（勝敗が確定しているものをぶち込む）
    for (i = 0; i < SIZE; i += 1){//-1で初期化
        to_check_index_table[i] = -1;
        checked_index_table[i] = -1;
    }
    for (i = 0; i < SIZE; i += 1){
        if (condition_array[i] != 1){
            to_check_index_table[c] = i;
            c += 1;
        }
    }
 


    int now_index = 0;
    unsigned int now_id = 0;
    int next_index = 0;
    unsigned int next_id = 0;
    int to_check_id = 0;
    unsigned to_check_index = 0;
    int layer = 1;

    int count = 0;
   
    while(to_check_index_table[0] != -1){//to_check_index_tableが空になるまでループ
        //to_check_index_tableの中身をchecked_index_tableに移す
        for (i = 0; i < SIZE; i += 1){
            checked_index_table[i] = to_check_index_table[i];
            count += 1;
            if (to_check_index_table[i] == -1){
                break;
            }
        }
        for (i = 0; i < SIZE; i += 1){//to_check_tableを-1で初期化
            to_check_index_table[i] = -1;
        }
        printf("%d\n", count);
        //checked_index_tableの中身について、その1個前のマスの勝敗が確定するかをチェック
        c = 0;
        for (i = 0; i < SIZE; i += 1){
            if (checked_index_table[i] == -1){
                break;
            }
            now_index = checked_index_table[i];//checked_index_tableの中身
            for (j = 0; j < DATA_LENGTH; j += 1){
                to_check_id = inv_graph_table[now_index][j];
                if (to_check_id == 0){
                    break;
                }
                to_check_index = hash_search(dictionary,to_check_id);//checked_index_tableの中身に遷移出来る状態
                if (condition_array[to_check_index] == 1){//その状態の勝敗が未確定（引き分け）だったならこれから勝敗が決まるかをチェック
                    //next_condition_array[to_check_index][0]:to_check_indexから遷移できる状態のうち相手から見て負けの状態の数
                    //next_condition_array[to_check_index][0]:to_check_indexから遷移できる状態のうち相手から見て引き分けの状態の数
                    //next_condition_array[to_check_index][0]:to_check_indexから遷移できる状態のうち相手から見て勝ちの状態の数
                    next_condition_table[to_check_index][condition_array[now_index]] += 1;
                    if (next_condition_table[to_check_index][0] > 0){//遷移先に相手の負け状態が1つでもあれば勝ち
                        condition_array[to_check_index] = 2;
                        to_check_index_table[c] = to_check_index;//新たに勝敗が確定したのでこの状態にたどり着く状態をこれからチェック
                        c += 1;
                        max_edges_to_end_table[to_check_index] = layer;
                    }
                    else if (next_condition_table[to_check_index][2] == edge_num_array[to_check_index]){
                        //遷移先のすべてが相手の勝ちなら負け
                        condition_array[to_check_index] = 0;
                        to_check_index_table[c] = to_check_index;
                        c += 1;
                        max_edges_to_end_table[to_check_index] = -layer;
                    }
                }
            }
        }
        layer += 1;
    }


    int best_id = 0;
    int tmp_max_transition_to_end = 0;
    

    //indexを走査することで全状態を走査
    for (i = 0; i < SIZE; i += 1){
        now_id = inv_dictionary[i];
        now_index = hash_search(dictionary,now_id);

        /*勝ち盤面なら遷移先の（相手目線の）負け盤面で決着手数の短いものに遷移*/
        if (condition_array[now_index] == 2){
            best_id = 0;
            tmp_max_transition_to_end = SIZE;
            //遷移先の状態を確認
            for (j = 0; j < DATA_LENGTH; j += 1){
                next_id = graph_table[now_index][j];//next_id:遷移先
                if (next_id == 0){
                    break;
                }
                next_index = hash_search(dictionary,next_id);
                if (condition_array[next_id] == 0){//遷移先が相手視点で負けなら・・・
                    if (max_edges_to_end_table[next_id] < tmp_max_transition_to_end){
                        tmp_max_transition_to_end = max_edges_to_end_table[next_id];
                        best_id = next_id;
                    }
                }   
            }
            best_table[now_id] = best_id;
        }
        /*負け盤面なら遷移先の（相手目線の）負け盤面で決着手数の短いものに遷移*/
        else if (condition_array[now_index] == 0){
            best_id = 0;
            tmp_max_transition_to_end = SIZE;

            for (j = 0; j < DATA_LENGTH; j += 1){
                next_id = graph_table[now_index][j];
                if (next_id == 0){
                    break;
                }
                next_index = hash_search(dictionary,next_id);
                if (condition_array[next_id] == 2){
                    if (max_edges_to_end_table[next_id] < tmp_max_transition_to_end){
                        tmp_max_transition_to_end = max_edges_to_end_table[next_id];
                        best_id = next_id;
                    }
                } else{//負け盤面なら遷移先全てが相手の勝ち盤面のはず
                    printf("err");
                }  
            }
            best_table[now_id] = best_id;
        }
        else{
            best_id = 0;
            tmp_max_transition_to_end = SIZE;

            for (j = 0; j < DATA_LENGTH; j += 1){
                next_id = graph_table[now_index][j];
                if (next_id == 0){
                    break;
                }
                next_index = hash_search(dictionary,next_id);
                if (condition_array[next_id] == 1){//引き分け盤面なら適当に遷移
                    tmp_max_transition_to_end = max_edges_to_end_table[next_id];
                    best_id = next_id;  
                    break;
                }  
            }
            best_table[now_id] = best_id;
        }
    }
}
