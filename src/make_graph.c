
#include "make_graph.h"
#include "hash_table.h"
#include "game.h"
#include "utility.h"
#include "file_io.h"
#include <stdio.h>
#include <stdlib.h>


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

    unsigned int *next_state_ids = (unsigned int*)malloc(sizeof(int)*DATA_LENGTH);
    
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


    printf("Making the dictionary...\n");
   
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
                                    
                                    for (int i = 0; i < DATA_LENGTH; i += 1){
                                        if (next_state_ids_for_black[i] == END){
                                            break;
                                        }
                                        graph_table[black_state_index][i] = next_state_ids_for_black[i];
                                    }

                                    
                                    
                                    for (int i = 0; i < DATA_LENGTH; i += 1){
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
    int *end_index = (int*)malloc(sizeof(int)*SIZE);
    unsigned int from_id;
    int to_index;
    for (int i = 0; i < SIZE; i += 1){
        for (int j = 0; j < DATA_LENGTH; j += 1){//i(index)からgraph[i][j](key)に遷移する
        //inv_graphにgraph[i][j](index)からi(key)への辺を張る
            if (graph_table[i][j] == END){
                break;
            }
            from_id = inv_dictionary[i];
            to_index = hash_search(dictionary ,graph_table[i][j]);
            inv_graph_table[to_index][end_index[to_index]] = from_id;
            end_index[to_index] += 1;
        }
    }

    free(end_index);
    printf("\nDone!\n");    
}

void edge_num_count(DataItem **dict, unsigned int inv_graph_table[][DATA_LENGTH], unsigned int edge_num_array[]){
    printf("Counting edges...\n");

    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < DATA_LENGTH; j++){
            if (inv_graph_table[i][j] == 0)
                break;

            edge_num_array[i] += 1;
        }
    }
    printf("Done!\n");
}

void generate_and_save_set(DataItem **dict, unsigned int inv_dict[], unsigned int graph_table[][DATA_LENGTH], unsigned int inv_graph_table[][DATA_LENGTH], unsigned int condition_array[])
{

    for (int i = 0; i < SIZE; i++)
    {
        condition_array[i] = 1;
    }

    hash_init(dict);

    make_dictionary(dict, inv_dict);

    make_graph(dict, inv_dict, graph_table, inv_graph_table, condition_array);

    printf("Saving set...\n");

    save_hash_table(dict, DICT_PATH);
    save_int_array(inv_dict, INV_DICT_PATH);
    save_int_table(graph_table, GRAPH_TABLE_PATH);
    save_int_table(inv_graph_table, INV_GRAPH_TABLE_PATH);
    save_int_array(condition_array, CONDITION_ARRAY_PATH);

    printf("Saved!\n");
}

void load_set(DataItem **dict, unsigned int inv_dict[], unsigned int graph_table[][DATA_LENGTH], unsigned int inv_graph_table[][DATA_LENGTH], unsigned int condition_array[])
{

    printf("Loading set...\n");

    hash_init(dict);
    load_hash_table_from_file(dict, DICT_PATH);
    load_int_array_from_file(inv_dict, INV_DICT_PATH);
    load_int_table_from_file(graph_table, GRAPH_TABLE_PATH);
    load_int_table_from_file(inv_graph_table, INV_GRAPH_TABLE_PATH);
    load_int_array_from_file(condition_array, CONDITION_ARRAY_PATH);

    printf("Done!\n");
}