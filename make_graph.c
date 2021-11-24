
#include "make_graph.h"
#include "hash_table.h"
#include "main.h"

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

int encode_board(int board[][5]) {
  int komacount = 0;
  int board_id;
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
  
  board_id = sub_id1 * 64 + sub_id2;
  return board_id;
}

void decode_board_id(int board_id, int board[][5]) {
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


int relative_move(int board[5][5], Point cur, Vector move_vec, int turn){
    
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
            return encode_board(new_board);
        }

    }

    return 0;

}

int* next_board_ids(int board_id, int turn){
    int board[5][5] = {};
    int next_board[5][5] = {};

    int *next_board_ids = malloc(sizeof(int)*MAX_TRANSITION);
    
    //初期化
    for(int i = 0; i < MAX_TRANSITION; i++){
        next_board_ids[i] = -1;
    }
    
    int count = 0; 
    decode_board_id(board_id,board);

    //すでに勝負がついてる盤面はどこにも遷移しない
    if(judge(board) != 0){
        return next_board_ids;
    }


    for(int i = 0 ; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(board[i][j] == turn){
                //iはy軸走査,jはx軸走査なので逆になります
                Point cur = {j, i};
                int next_board_id;

                //全方位確認する
                for(int dx = -1; dx < 2; dx++){
                    for(int dy = -1; dy < 2; dy++){
                        if(dx == 0 && dy == 0) continue;

                        Vector move_vec = {dx,dy};
                        next_board_id = relative_move(board, cur, move_vec, turn);
                        
                        

                        if(next_board_id != 0){
                            next_board_ids[count] = next_board_id;
                            count++;
                        }
                        
                    }
                }
            }
        }
    }

    return next_board_ids;
}

void make_graph(DataItem **black_table,DataItem **inv_black_table,DataItem **white_table, DataItem **inv_white_table, DataItem **black_condition_table, DataItem **white_condition_table){

    int w1,w2,w3,b1,b2,b3;
    int *board_num_array[6] = {&w1,&w2,&w3,&b1,&b2,&b3};
    int board[5][5] = {};


    
    int count = 0;

    printf("Constructing graphs...\n");
   
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

                            int judge_of_white = judge_one_side(board,WHITE);
                            int judge_of_black = judge_one_side(board,BLACK);

                            //どちらも勝っている状態、ではないとき
                            if( judge_of_white == 0 || judge_of_black == 0){
                                int board_id = encode_board(board);
                               
                                count++;

                                //白を動かしたときの遷移先のidの配列を保存
                                int *white_next_board_ids = next_board_ids(board_id,WHITE);
                                
                                hash_insert(white_table, board_id, white_next_board_ids);
                            
                                //エッジを逆方向にして保存
                                for(int i = 0; i < MAX_TRANSITION; i++){
                                    if(white_next_board_ids[i] == -1) break;

                                    hash_append_data(inv_white_table, white_next_board_ids[i], board_id);

                                }

                                //黒を動かしたときの遷移先のidの配列を保存
                                int *black_next_board_ids = next_board_ids(board_id,BLACK);
                                hash_insert(black_table, board_id, black_next_board_ids);

                                //エッジを逆方向にして保存
                                for(int i = 0; i < MAX_TRANSITION; i++){
                                    if(black_next_board_ids[i] == -1) break;

                                    hash_append_data(inv_black_table, black_next_board_ids[i], board_id);
                                }

                                //その色が勝っていれば1が,負けていれば-1が入る
                                int black_cond[MAX_TRANSITION] = {};
                                int white_cond[MAX_TRANSITION] = {};
                                for (int i = 0; i < MAX_TRANSITION; i++){
                                    black_cond[i] = judge_of_black - judge_of_white;
                                    white_cond[i] = judge_of_white - judge_of_black;
                                }
                        
                                hash_insert(black_condition_table,board_id,black_cond);
                                hash_insert(white_condition_table,board_id,white_cond);


                            }
                        }
                    }
                }
            }
        }

        
    }


    printf("\nDone!\n");

    printf("count: %d\n",count);


    
    
}

void save_table(DataItem **table,char* file_path){
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

void reconstruct_graph_from_file(DataItem **table, char* file_path){
    FILE *fpr = fopen(file_path,"rb");

    DataItem *data_item = calloc(SIZE,sizeof(DataItem));

    for(int i = 0; i < STATE_NUM; i++){

        fread(&data_item[i], sizeof(DataItem), 1, fpr);
        hash_insert(table,data_item[i].key,data_item[i].data);
    }

    free(data_item);

    fclose(fpr);

}



