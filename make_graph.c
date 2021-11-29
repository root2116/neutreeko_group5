
#include "make_graph.h"
#include "hash_table.h"
#include "main.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>

// long int to_int(int **board_array){
//     long int board_num = 0;

//     for(int i = 0; i < 6; i++){
//         board_num += *board_array[i];
//         board_num *= 100;
//     }

//     return board_num;
// }

void generate_board_from_array(int board[5][5], int **board_num_array){
    
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            board[i][j] = 0;
        }
    }

    int x,y;
    for(int i = 0; i < 6; i++){
        x = *board_num_array[i] % 5;
        y = *board_num_array[i] / 5;
        if(i < 3){
            board[y][x] = WHITE;
        }else{
            board[y][x] = BLACK;
        }
        
    }   
}

int get_color_from_state_id(unsigned int state_id) {
  int turn = state_id >> 31;
  return turn+1;
}

unsigned int encode_board(int board[][5], int turn) {
  int komacount = 0;
  unsigned int board_id;
  int sub_id1 = 0;
  int sub_id2 = 0;
  
  /*盤面に対して別々にエンコード*/
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (board[i][j] == 1) {
        sub_id1 += 1 << (24 - (5 * i + j));
        komacount++;
      }
      else if (board[i][j] == 2) {
        sub_id1 += 1 << (24 - (5 * i + j));
        sub_id2 += 1 << (5 - komacount);
        komacount++;
      } else {}
    }
  }
  
  board_id = ((turn - 1)<< 31) + sub_id1 * 64 + sub_id2;
  return board_id;
}


void decode_state_id(unsigned int board_id, int board[][5]) {
  /*sub_id2が黒白の順番、sub_id1が駒のあるなし*/
  int sub_id2 = board_id % 64;
  int sub_id1 = board_id / 64;

  for (int i = 24; i >= 0; i--) {
    if ((sub_id1 % 2) == 1) {
      if ((sub_id2 % 2) == 0) {
        board[(i / 5)][(i % 5)] = 1;
      } else {
        board[(i / 5)][(i % 5)] = 2;
      }
      sub_id2 /= 2;
    }else{
        board[(i / 5)][(i % 5)] = 0;
    }
    sub_id1 /= 2;
  }
  return;
}


unsigned int relative_move(int board[5][5], Point cur, Vector move_vec, int turn){
    
    //コピー
    Point front = cur;
    Move move;
    int new_board[5][5] = {};

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            new_board[i][j] = board[i][j];
        }
    }

    while(1){
        
        add_vec_to_point(&front, &move_vec,&front);

        if(is_on_wall(front) == 1){
            break;
        }
        move.start = cur;
        move.end = front;
        
        if(move_piece(new_board,move,turn)){
            if(turn == WHITE){
                return encode_board(new_board,BLACK);
            }else{
                return encode_board(new_board,WHITE);
            }
        }

    }

    return 0;

}

unsigned int* generate_next_state_ids(unsigned int state_id){
    int board[5][5] = {};
    // int next_board[5][5] = {};

    unsigned int *next_state_ids = malloc(sizeof(int)*DATA_LENGTH);
    
    //初期化
    for(int i = 0; i < DATA_LENGTH; i++){
        next_state_ids[i] = END;
    }
    
    int count = 0; 
    decode_state_id(state_id,board);

    //すでに勝負がついてる盤面はどこにも遷移しない
    if(judge(board) != 0){
        return next_state_ids;
    }

    int color = get_color_from_state_id(state_id);

    for(int i = 0 ; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(board[i][j] == color){
                //iはy軸走査,jはx軸走査なので逆になります
                Point cur = {j, i};
                unsigned int next_state_id;

                //全方位確認する
                for(int dx = -1; dx < 2; dx++){
                    for(int dy = -1; dy < 2; dy++){
                        if(dx == 0 && dy == 0) continue;

                        Vector move_vec = {dx,dy};
                        next_state_id = relative_move(board, cur, move_vec, color);
                        
                        

                        if(next_state_id != 0){
                            next_state_ids[count] = next_state_id;
                            count++;
                        }
                        
                    }
                }
            }
        }
    }

    return next_state_ids;
}


void make_dictionary(DataItem **dictionary, unsigned int inv_dictionary[SIZE]){

    int w1,w2,w3,b1,b2,b3;
    int *board_num_array[6] = {&w1,&w2,&w3,&b1,&b2,&b3};
    int board[5][5] = {};
    
    int count = 0;

    unsigned int black_state_id;
    unsigned int white_state_id;
    int judge_of_white;
    int judge_of_black;


    printf("makeing dictionary...\n");
   
    for(w1 = 0; w1 < 25; w1++){
        for(w2 = w1 + 1; w2 < 25; w2++){
            fprintf(stderr, "\r[%3d / 100]",(count*100/STATE_NUM));
            for(w3 = w2 + 1; w3 < 25; w3++){
                for(b1 = 0; b1 < 25; b1++){
                    if(b1 == w1 || b1 == w2 || b1 == w3) continue;
                    for(b2 = b1 + 1; b2 < 25; b2++){
                        if(b2 == w1 || b2 == w2 || b2 == w3) continue;
                        for(b3 = b2 + 1; b3 < 25; b3++){
                            if(b3 == w1 || b3 == w2 || b3 == w3) continue;
                        
                            generate_board_from_array(board,board_num_array);

                            judge_of_white = judge_one_side(board,WHITE);
                            judge_of_black = judge_one_side(board,BLACK);
                            
                            //どちらも勝っている状態、ではないとき
                            if( judge_of_white == 0 || judge_of_black == 0){
                                

                                black_state_id = encode_board(board,BLACK);
                                white_state_id = encode_board(board, WHITE);
                                

                                hash_insert(dictionary, black_state_id, count);
                                inv_dictionary[count] = black_state_id;
                                count += 1;

                                hash_insert(dictionary, white_state_id, count);
                                inv_dictionary[count] = white_state_id;
                                count += 1;
                            }
                        }
                    }
                }
            }
        }  
    }
    printf("\nDone!\n");  
}



void make_graph(DataItem **dictionary, unsigned int inv_dictionary[SIZE],unsigned int graph_table[SIZE][DATA_LENGTH], unsigned int inv_graph_table[SIZE][DATA_LENGTH], unsigned int condition_array[]){

    int w1,w2,w3,b1,b2,b3;
    int *board_num_array[6] = {&w1,&w2,&w3,&b1,&b2,&b3};
    int board[5][5] = {};
    int black_state_index = 0;
    int white_state_index = 0;
    int i;
    int loop_count = 0;
    

    printf("Constructing graphs...\n");
   
    for(w1 = 0; w1 < 25; w1++){
        for(w2 = w1 + 1; w2 < 25; w2++){
            fprintf(stderr, "\r[%3d / 100]",(2*loop_count*100/STATE_NUM));
            for(w3 = w2 + 1; w3 < 25; w3++){
                for(b1 = 0; b1 < 25; b1++){
                    if(b1 == w1 || b1 == w2 || b1 == w3) continue;
                    for(b2 = b1 + 1; b2 < 25; b2++){
                        if(b2 == w1 || b2 == w2 || b2 == w3) continue;
                        for(b3 = b2 + 1; b3 < 25; b3++){
                            if(b3 == w1 || b3 == w2 || b3 == w3) continue;
                            

                            loop_count++;
                            generate_board_from_array(board,board_num_array);

                            int judge_of_white = judge_one_side(board,WHITE);
                            int judge_of_black = judge_one_side(board,BLACK);

                            if( judge_of_white == 0 || judge_of_black == 0){//両者勝ちの盤面ではない
                                // その色が勝っていれば2が,負けていれば0が、引き分け1
                                unsigned int black_state_id = encode_board(board,BLACK);
                                unsigned int white_state_id = encode_board(board,WHITE);
                                black_state_index = hash_search(dictionary, black_state_id);
                                white_state_index = hash_search(dictionary,white_state_id);  
                                condition_array[black_state_index] = (judge_of_black - judge_of_white) + 1;
                                condition_array[white_state_index] = -(judge_of_black - judge_of_white) + 1;

                                //どちらかが勝っている状態、ではないとき(試合継続、遷移あり)
                                if( judge_of_white == 0 && judge_of_black == 0){
                                    unsigned int *next_state_ids_for_black = generate_next_state_ids(black_state_id);
                                    unsigned int *next_state_ids_for_white = generate_next_state_ids(white_state_id);
                                    
                                    for (i = 0; i < DATA_LENGTH; i += 1){
                                        if (next_state_ids_for_black[i] == END){
                                            break;
                                        }
                                        graph_table[black_state_index][i] = next_state_ids_for_black[i];
                                    }

                                    
                                    
                                    for (i = 0; i < DATA_LENGTH; i += 1){
                                        if (next_state_ids_for_white[i] == END){
                                            break;
                                        }
                                        graph_table[white_state_index][i] = next_state_ids_for_white[i];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }    
    }
    int *end_index = malloc(sizeof(int)*SIZE);
    unsigned int from_key;
    int to_index;
    for (int i = 0; i < SIZE; i += 1){
        for (int j = 0; j < DATA_LENGTH; j += 1){//i(index)からgraph[i][j](key)に遷移する
        //inv_graphにgraph[i][j](index)からi(key)への辺を張る
            from_key = inv_dictionary[i];
            to_index = hash_search(dictionary ,graph_table[i][j]);
            inv_graph_table[to_index][end_index[to_index]] = from_key;
            end_index[to_index] += 1;
        }
    }

    free(end_index);
    printf("\nDone!\n");    
}



// void remove_unreachable_states(DataItem **graph_table,DataItem **condition_table){
//     DataItem *seen = calloc(SIZE, sizeof(DataItem));

//     hash_init((DataItem**)seen);
//     graph_search((DataItem**)seen,BLACK_INIT_STATE_ID,graph_table);

//     graph_search((DataItem**)seen,WHITE_INIT_STATE_ID,graph_table);

//     for(int i = 0; i < SIZE; i++){
        
//         recursive_delete((DataItem**)seen,graph_table[i], graph_table,condition_table);
//     }

//     free(seen);
// }


// void graph_search(DataItem **seen, unsigned int init_state_id, DataItem **graph_table){

//     Queue *queue = malloc(sizeof(Queue));
//     queue_init(queue);


//     unsigned int seen_data[DATA_LENGTH] = {};
//     seen_data[0] = 1; 
//     hash_insert(seen, init_state_id,1);

//     enqueue(queue,init_state_id);
    

//     while(!queue_empty(queue)){
//         unsigned int state_id = dequeue(queue);
        
//             unsigned int *data = hash_search(graph_table, state_id);
//         for(int i = 0; i < DATA_LENGTH; i++){
//             if(data[i] == END) break;

//             if(hash_search(seen,data[i]) != NULL) continue;

//             hash_insert(seen,data[i],seen_data);
//             enqueue(queue,data[i]);
            
//         }
//     }

    


// }

// void recursive_delete(DataItem **seen, DataItem *data_item, DataItem **graph_table, DataItem **condition_table){
//     if(data_item == NULL) return;
   
//     if (hash_search(seen, data_item->key) == NULL)
//     {
//         hash_delete(graph_table, data_item->key);
//         hash_delete(condition_table, data_item->key);
        
//     }
//     recursive_delete(seen, data_item->next, graph_table, condition_table);
// }

void save_hash_table(DataItem **table,char* file_path){
    FILE *fpw = fopen(file_path,"wb");

    for(int i = 0; i < SIZE; i++){
        if(table[i] == NULL) continue;

        recursive_save(table[i],fpw);
    }


    fclose(fpw);

}
void recursive_save(DataItem *data_item, FILE *fpw){
    if(data_item->next == NULL){
        fwrite(data_item,sizeof(DataItem),1,fpw);

    }else{
        fwrite(data_item,sizeof(DataItem),1,fpw);
        recursive_save(data_item->next,fpw);
    }
}   

void save_int_table(unsigned int table[][DATA_LENGTH], char* file_path){
    FILE *fpw = fopen(file_path, "wb");

    fwrite(table,sizeof(int),SIZE*DATA_LENGTH,fpw);
    
    fclose(fpw);
}

void save_int_array(unsigned int array[], char* file_path){
    FILE *fpw = fopen(file_path, "wb");

    fwrite(array,sizeof(int),SIZE, fpw);
    
    fclose(fpw);
}

void reconstruct_hash_table_from_file(DataItem **table, char* file_path){
    FILE *fpr = fopen(file_path,"rb");

    DataItem *data_item = calloc(SIZE,sizeof(DataItem));

    for(int i = 0; i < STATE_NUM; i++){
        
        fread(&data_item[i], sizeof(DataItem), 1, fpr);
        if(data_item[i].key == 0){
            break;
        }
        hash_insert(table,data_item[i].key,data_item[i].data);
    }

    free(data_item);

    fclose(fpr);

}

void reconstruct_int_table_from_file(unsigned int table[][DATA_LENGTH], char* file_path){
    FILE *fpr = fopen(file_path, "rb");

    fread(table,sizeof(int),SIZE*DATA_LENGTH,fpr);

    fclose(fpr);
}

void reconstruct_int_array_from_file(unsigned int array[], char* file_path){
    FILE *fpr = fopen(file_path, "rb");

    fread(array, sizeof(int), SIZE, fpr);

    fclose(fpr);
}

void edge_num_count(DataItem **dict, unsigned int inv_graph_table[][DATA_LENGTH], unsigned int edge_num_array[])
{

    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            if(inv_graph_table[i][j] == 0) break;

            edge_num_array[i] += 1;
        }
    }
    
}


void generate_and_save_set(DataItem **dict,unsigned int inv_dict[], unsigned int graph_table[][DATA_LENGTH], unsigned int inv_graph_table[][DATA_LENGTH],unsigned int condition_array[] ){

    for (int i = 0; i < SIZE; i++)
    {
        condition_array[i] = 1;
    }

    hash_init(dict);

    make_dictionary(dict,inv_dict);

    make_graph(dict,inv_dict,graph_table, inv_graph_table,condition_array);

    printf("Saving...\n");
    
    save_hash_table(dict,"dict.dat");
    save_int_array(int_dict,"int_dict.dat");
    save_int_table(graph_table, "graph_table.dat");
    save_int_table(inv_graph_table, "inv_graph_table.dat");
    save_int_array(condition_array, "condition_array.dat");

    printf("Saved!\n");
}

void reconsturct_set(DataItem **dict, unsigned int inv_dict[], unsigned int graph_table[][DATA_LENGTH], unsigned int inv_graph_table[][DATA_LENGTH], unsigned int condition_array[]){
    reconstruct_hash_table_from_file(dict,"dict.dat");
    reconstruct_int_array_from_file(inv_dict,"inv_dict.dat");
    reconstruct_int_table_from_file(graph_table,"graph_table.dat");
    reconstruct_int_table_from_file(inv_graph_table,"inv_graph_table.dat");
    reconstruct_int_array_from_file(condition_array,"condition_array.dat");
}
