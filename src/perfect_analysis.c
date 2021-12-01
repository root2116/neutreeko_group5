
#include "perfect_analysis.h"
#include "hash_table.h"
#include "game.h"
#include "utility.h"
#include "file_io.h"

#include <stdio.h>
#include <stdlib.h>

unsigned int graph_table[SIZE][DATA_LENGTH];
unsigned int inv_graph_table[SIZE][DATA_LENGTH];
unsigned int condition_array[SIZE] = {};
unsigned int edge_num_array[SIZE] = {};
unsigned int best_array[SIZE] = {};
DataItem *dict[SIZE];
unsigned int inv_dict[SIZE] = {};
unsigned int next_condition_table[SIZE][3];


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


void make_dictionary(){

    int w1,w2,w3,b1,b2,b3;
    int *board_num_array[6] = {&w1,&w2,&w3,&b1,&b2,&b3};
    int board[5][5] = {};
    
    int count = 0;

    unsigned int black_state_id;
    unsigned int white_state_id;
    int judge_of_white;
    int judge_of_black;


    printf("Making a dictionary...\n");
   
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
                                

                                hash_insert(dict, black_state_id, count);
                                inv_dict[count] = black_state_id;
                                count += 1;

                                hash_insert(dict, white_state_id, count);
                                inv_dict[count] = white_state_id;
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



void make_graph(){

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
                                black_state_index = hash_search(dict, black_state_id);
                                white_state_index = hash_search(dict,white_state_id);  
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
            from_id = inv_dict[i];
            to_index = hash_search(dict ,graph_table[i][j]);
            inv_graph_table[to_index][end_index[to_index]] = from_id;
            end_index[to_index] += 1;
        }
    }

    free(end_index);
    printf("\nDone!\n");    
}

void edge_num_count(unsigned int graph_table[][DATA_LENGTH]){
   
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < DATA_LENGTH; j++){
            if (graph_table[i][j] == 0)
                break;

            edge_num_array[i] += 1;
        }
    }
}



void calculate_best()
{

    int *fixed_index_array = calloc(SIZE, sizeof(int));
    int *new_fixed_index_array = calloc(SIZE, sizeof(int));
    int *max_edges_to_end_table = calloc(SIZE, sizeof(int));

    printf("Calculating best moves...\n");

    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < 3; j++){
            next_condition_table[i][j] = 0;
        }
    }

    //new_fixed_index_array(初期状態)　の作成（勝敗が確定しているものをぶち込む）
    for (int i = 0; i < SIZE; i += 1)
    { //-1で初期化
        new_fixed_index_array[i] = -1;
        fixed_index_array[i] = -1;
    }
    int c = 0;
    for (int i = 0; i < SIZE; i += 1)
    {
        if (condition_array[i] != 1)
        {

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

    while (new_fixed_index_array[0] != -1)
    { //new_fixed_index_arrayが空になるまでループ
        //new_fixed_index_arrayの中身をfixed_index_arrayに移す
        for (int i = 0; i < SIZE; i++)
        {
            fixed_index_array[i] = new_fixed_index_array[i];
            count += 1;
            if (new_fixed_index_array[i] == -1)
            {
                break;
            }
        }
        for (int i = 0; i < SIZE; i++)
        { //to_check_tableを-1で初期化
            new_fixed_index_array[i] = -1;
        }

        fprintf(stderr, "\r1 / 2 [%3d / 100]", (count * 100 / MAX_FIXED));
        //fixed_index_arrayの中身について、その1個前のマスの勝敗が確定するかをチェック
        int end_index = 0;
        for (int i = 0; i < SIZE; i++)
        {
            if (fixed_index_array[i] == -1)
            {
                break;
            }

            now_index = fixed_index_array[i]; //fixed_index_arrayの中身

            for (int j = 0; j < DATA_LENGTH; j++)
            {
                uncertain_id = inv_graph_table[now_index][j];
                if (uncertain_id == 0)
                {
                    break;
                }
                uncertain_index = hash_search(dict, uncertain_id); //fixed_index_arrayの中身に遷移出来る状態
                if (condition_array[uncertain_index] == DRAW)
                { //その状態の勝敗が未確定（引き分け）だったならこれから勝敗が決まるかをチェック
                    //next_condition_array[uncertain_index][0]:uncertain_indexから遷移できる状態のうち相手から見て負けの状態の数
                    //next_condition_array[uncertain_index][1]:uncertain_indexから遷移できる状態のうち相手から見て引き分けの状態の数
                    //next_condition_array[uncertain_index][2]:uncertain_indexから遷移できる状態のうち相手から見て勝ちの状態の数

                    //now_indexはすでに勝敗が決まっていて、uncertain_indexからも遷移できる状態なので
                    next_condition_table[uncertain_index][condition_array[now_index]] += 1;
                    if (next_condition_table[uncertain_index][LOSE] > 0)
                    { //遷移先に相手の負け状態が1つでもあれば勝ち
                        fixed_index = uncertain_index;

                        condition_array[fixed_index] = WIN;
                        new_fixed_index_array[end_index] = fixed_index; //新たに勝敗が確定したのでこの状態にたどり着く状態をこれからチェック
                        end_index += 1;
                        max_edges_to_end_table[fixed_index] = layer;
                    }
                    else if (next_condition_table[uncertain_index][WIN] == edge_num_array[uncertain_index])
                    {
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

    fprintf(stderr, "\r1 / 2 [100 / 100]\n");

    int best_id = 0;
    int tmp_max_transition_to_end = 0;

    //indexを走査することで全状態を走査
    for (int i = 0; i < STATE_NUM; i += 1)
    {
        if(i % 100000 == 0){
            fprintf(stderr, "\r2 / 2 [%3d / 100]", ((i + 1) * 100 / STATE_NUM));
        }
        

        now_id = inv_dict[i];
        now_index = hash_search(dict, now_id);
    

        /*勝ち盤面なら遷移先の（相手目線の）負け盤面で決着手数の短いものに遷移*/
        if (condition_array[now_index] == WIN)
        {
            best_id = 0;
            tmp_max_transition_to_end = -SIZE;
            //遷移先の状態を確認
            for (int j = 0; j < DATA_LENGTH; j++)
            {
                next_id = graph_table[now_index][j]; //next_id:遷移先
                if (next_id == 0)
                {
                    break;
                }
                next_index = hash_search(dict, next_id);
                if (condition_array[next_index] == LOSE)
                { //遷移先が相手視点で負けなら・・・
                    if (max_edges_to_end_table[next_index] > tmp_max_transition_to_end)
                    {
                        tmp_max_transition_to_end = max_edges_to_end_table[next_index];
                        best_id = next_id;
                    }
                }
            }
            best_array[now_index] = best_id;
        }
        /*負け盤面なら遷移先の（相手目線の）負け盤面で決着手数の短いものに遷移*/
        else if (condition_array[now_index] == LOSE)
        {
            best_id = 0;
            tmp_max_transition_to_end = -SIZE;

            for (int j = 0; j < DATA_LENGTH; j++)
            {
                next_id = graph_table[now_index][j];
                if (next_id == 0)
                {
                    break;
                }
                next_index = hash_search(dict, next_id);
                if (condition_array[next_index] == WIN)
                {
                    if (max_edges_to_end_table[next_index] > tmp_max_transition_to_end)
                    {
                        tmp_max_transition_to_end = max_edges_to_end_table[next_index];
                        best_id = next_id;
                    }
                }
                else
                { //負け盤面なら遷移先全てが相手の勝ち盤面のはず
                    printf("err\n");
                }
            }
            best_array[now_index] = best_id;
        }
        else
        {
            best_id = 0;
            tmp_max_transition_to_end = SIZE;

            for (int j = 0; j < DATA_LENGTH; j++)
            {
                next_id = graph_table[now_index][j];
                if (next_id == 0)
                {
                    break;
                }
                next_index = hash_search(dict, next_id);
                if (condition_array[next_index] == 1)
                { //引き分け盤面なら適当に遷移
                    tmp_max_transition_to_end = max_edges_to_end_table[next_index];
                    best_id = next_id;
                    break;
                }
            }
            best_array[now_index] = best_id;
        }
    }

    free(fixed_index_array);
    free(new_fixed_index_array);
    free(max_edges_to_end_table);

    fprintf(stderr, "\r2 / 2 [100 / 100]");

    printf("\nDone!\n");
}

void convert_best_array(short int best_move_array[])
{
    unsigned int current_id, best_id;
    int current_board[5][5], best_board[5][5];
    short int start_int, end_int, move_int;
    for (int index = 0; index < STATE_NUM; index++)
    {
        start_int = 0;
        end_int = 0;
        current_id = inv_dict[index];

        best_id = best_array[index];
        if (best_id == 0)
            continue;

        decode_state_id(current_id, current_board);
        decode_state_id(best_id, best_board);

        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                //手の始点をさがす
                if (best_board[i][j] == EMPTY && (current_board[i][j] != best_board[i][j]))
                {
                    start_int = (5 - i) * 10 + j;
                }

                //手の終点をさがす
                if (current_board[i][j] == EMPTY && (current_board[i][j] != best_board[i][j]))
                {
                    end_int = (5 - i) * 10 + j;
                }
            }
        }

        move_int = start_int * 100 + end_int;

        best_move_array[index] = move_int;
    }
}

void generate_and_save_set()
{

    for (int i = 0; i < SIZE; i++)
    {
        condition_array[i] = 1;
    }

    hash_init(dict);

    make_dictionary();

    make_graph();

    printf("Saving set...\n");

    save_hash_table(dict, DICT_PATH);
    save_int_array(inv_dict, INV_DICT_PATH);
    save_int_table(graph_table, GRAPH_TABLE_PATH);
    save_int_table(inv_graph_table, INV_GRAPH_TABLE_PATH);
    save_int_array(condition_array, CONDITION_ARRAY_PATH);

    printf("Saved!\n");
}

void load_set()
{

    printf("Loading set...\n");

    hash_init(dict);
    init_table(graph_table);
    init_table(inv_graph_table);

    load_hash_table_from_file(dict, DICT_PATH);
    load_int_array_from_file(inv_dict, INV_DICT_PATH);
    load_int_table_from_file(graph_table, GRAPH_TABLE_PATH);
    load_int_table_from_file(inv_graph_table, INV_GRAPH_TABLE_PATH);
    load_int_array_from_file(condition_array, CONDITION_ARRAY_PATH);

    printf("Done!\n");
}

void generate_and_save_best_move_array()
{


    load_set();

    edge_num_count(graph_table);

    calculate_best();

    printf("Converting best_array...\n");

    short int *best_move_array = calloc(SIZE, sizeof(short int));

    convert_best_array(best_move_array);

    printf("Saving a best move array\n");

    save_short_array(best_move_array, BEST_MOVE_ARRAY_PATH);

    printf("Saved!\n");

}

void init_set(){

    for (int i = 0; i < SIZE; i++)
    {
        condition_array[i] = 1;
    }


    hash_init(dict);
    init_table(graph_table);
    init_table(inv_graph_table);


}



void prepare_for_game(){

    printf("Initializing set...\n");
    init_set();

    make_dictionary();

    make_graph();

    edge_num_count(graph_table);

    calculate_best();

    printf("Converting best_array...\n");

    short int *best_move_array = calloc(SIZE, sizeof(short int));

    convert_best_array(best_move_array);

    printf("Saving a best move array and the dictionary...\n");

    save_short_array(best_move_array, BEST_MOVE_ARRAY_PATH);
    save_hash_table(dict, DICT_PATH);


    printf("Saved!\n");
}







