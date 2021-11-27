#include "main.h"
#include "hash_table.h"
#include "make_graph.h"
#include "test_util.h"
#include "cal_best.h"
#include<stdio.h>
#include<stdlib.h>
#include <assert.h>

int gcd(int x, int y){
    
    if(y == 0){
        return x;
    }else{
        return gcd(y, x % y);
    }
}
//char to int
int ctoi(char c){
    if (c >= '0' && c <= '9'){
        return c - '0';
    }
    return 0;
}

void parser(char* input, Move* move){

    move->start.x = input[1] - 65;
    move->start.y = 5 - ctoi(input[0]);
    move->end.x = input[3] - 65;
    move->end.y = 5 - ctoi(input[2]);

    
}

//２つの点a,bからベクトルabを作る
void make_vec(Point *a, Point *b, Vector *v){
    v->x = b->x - a->x;
    v->y = b->y - a->y;
}

//点にベクトルを足した点を返す
void add_vec_to_point(Point *a, Vector *v, Point *b){ 
    b->x = a->x + v->x;
    b->y = a->y + v->y;
}

/*ゲームが終わっているか判定する関数*/

int judge_one_side(int board[5][5], int color){
    /*黒をチェック*/
    int out = 1; /*返り値*/

    /*黒のコマがある場所を確認　（2次元配列を1次元に落としてます）*/
    int tmp[3];
    int c = 0;
    int i, j;

    for (i = 0; i < 5; i += 1)
    {
        for (j = 0; j < 5; j += 1)
        {
            if (board[i][j] == color)
            {
                tmp[c] = 5 * i + j;
                c += 1;
            }
        }
    }

    int sa1, sa2;
    sa1 = tmp[1] - tmp[0];
    sa2 = tmp[2] - tmp[1];

    if (sa1 == sa2)
    {
        if (sa1 == 1)
        { /*横*/
            if (tmp[0] / 5 == tmp[1] / 5 && tmp[1] / 5 == tmp[2] / 5)
            {
                return out;
            }
        }
        if (sa1 == 5)
        { /*縦*/
            return out;
        }
        if (sa1 == 6)
        { /*斜め　左上->右下*/
            if (tmp[0] / 5 + 1 == tmp[1] / 5 && tmp[1] / 5 + 1 == tmp[2] / 5)
            {
                return out;
            }
        }
        if (sa1 == 4)
        { /*斜め　右上->左下*/
            if (tmp[0] / 5 + 1 == tmp[1] / 5 && tmp[1] / 5 + 1 == tmp[2] / 5)
            {
                return out;
            }
        }
    }

    return 0;
}


int judge (int board[5][5]){
    int result = judge_one_side(board,BLACK);

    if(result != 0){
        return 1;
    }

    result = judge_one_side(board,WHITE);

    if(result != 0){
        return 2;
    }

    return 0;

}


int is_on_wall(Point p){
    if (p.x == -1 || p.x == 5 || p.y == -1 || p.y == 5){
        return 1;
    }else{
        return 0;
    }
}

int is_out_board(Point p){
    if (p.x < 0 || p.x > 4 || p.y < 0 || p.y > 4)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


// 1 : 動かせる, 0: 動かせない
int can_move(int board[5][5], Move move, int turn){
    //現在のターンの人が動かせる駒でなければ動かせない
    if(board[move.start.y][move.start.x] != turn) return 0;

    //動かす先に駒がすでに存在したら動かせない
    if(board[move.end.y][move.end.x] != EMPTY) return 0;

  
    Vector move_vec;
    make_vec(&move.start, &move.end, &move_vec);

    int vec_gcd = gcd(abs(move_vec.x), abs(move_vec.y));

    //成分が互いに素であるベクトル
    Vector coprime_move_vec = { move_vec.x / vec_gcd ,move_vec.y / vec_gcd };
        

    //縦横斜めのベクトルではなかったら動かせない.( (0,±1),(±1,0),(±1,±1)以外のベクトルの長さの２乗は2より大きくなる)
    if(coprime_move_vec.x * coprime_move_vec.x + coprime_move_vec.y * coprime_move_vec.y > 2) return 0;


    Point front;
    add_vec_to_point(&move.end, &coprime_move_vec, &front);

    Point scan;
    add_vec_to_point(&move.start, &coprime_move_vec, &scan);
  

    //射線上に駒がいたら動かせない
    while(scan.x != move.end.x || scan.y != move.end.y){
        if(board[scan.y][scan.x] != EMPTY) return 0;

        add_vec_to_point(&scan, &coprime_move_vec, &scan);
    }
    

    //目の前が壁だったらOK
    if(is_on_wall(front)){
        return 1;
    }else if(is_out_board(front)){
        return 0;
    }
    else if(board[front.y][front.x] != EMPTY){ // 目の前が駒ならOK
        return 1;
    }else{ //それ以外だったら動かせない
        return 0;
    }  
   
}


int move_piece(int board[5][5], Move move,int turn){
    if(can_move(board,move,turn)){
        int piece = board[move.start.y][move.start.x];
        board[move.start.y][move.start.x] = EMPTY;
        board[move.end.y][move.end.x] = piece;
        return 1;
    }else{
        return 0;
    }
}




void update_turn(int* turn,int* turn_count){
    *turn_count = *turn_count + 1;
    if(*turn == 1){
        *turn = 2;
    }else{
        *turn = 1;
    }
}


void display_board(int board[5][5]) {
  //読み込み
  int width = 6;
  char boardmap[width][width];

  boardmap[0][0] = ' ';

  

  for (int i = 1; i < width; i++) {
      boardmap[0][i] = 'A' + i - 1;
      boardmap[i][0] = '6' - i;
  }
  
  //新しいマップ
  for (int i = 1; i < width; i++) {
    for (int j = 1; j < width; j++) {
      if (board[i - 1][j - 1] == BLACK) {
        boardmap[i][j] = 'O';
      }else if(board[i - 1][j - 1] == WHITE){
          boardmap[i][j] = '@';
      }else {
        boardmap[i][j] = ' ';
      }
    }
  }

  //プリント

  printf("\n");

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < width; j++) {
      printf("%c|", boardmap[i][j]);
    }
    printf("\n");
  }

  printf("\n");
}

DataItem *graph_table[SIZE];
DataItem *inv_graph_table[SIZE];
DataItem *condition_table[SIZE];
DataItem *revived_graph_table[SIZE];


int main(int argc,char *argv[]){

    int board[5][5] = {
        {0, 2, 0, 2, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 2, 0, 0},
        {0, 1, 0, 1, 0}};

    //テスト----------------------------------------------------------------

    printf("int: %lu bytes\n",sizeof(int));
    hash_init(graph_table);
    hash_init(inv_graph_table);
    hash_init(condition_table);
    hash_init(revived_graph_table);

    int moves[DATA_LENGTH] = {};
    for(int i = 0; i < DATA_LENGTH; i++){
        moves[i] = -1;
    }

    


    //generate_board_from_array
    int w1,w2,w3,b1,b2,b3;
    int *board_num_array[6] = {&w1,&w2,&w3,&b1,&b2,&b3};
    w1 = 1; w2 = 3; w3 = 17; b1 = 7; b2 = 21; b3 = 23;
    
    int test_board[5][5] = {};
    generate_board_from_array(test_board,board_num_array);
    assert(is_same_board(board,test_board));

    //encode_board
    unsigned int encoded = encode_board(board,WHITE);
    printf("%u\n", encoded);
    assert(encoded == 2826969780);

    //decode_board_id
    int decoded[5][5] = {};
    decode_state_id(encoded, decoded);
    display_board(decoded);
    assert(is_same_board(board,decoded));


    //relative_move 
    Point cur = {1,4};
    Vector move_vec = {0,-1};
    assert(relative_move(board, cur, move_vec, BLACK) == 2843746482);

    //next_board_ids
    
    unsigned int *next_state_ids_for_black = generate_next_state_ids(1937768462);

    unsigned int expectations[] = {
        4076896270,
        4076863758,
        4211081227,
        4078960654,
        4076865550};

    for(int i = 0; i < 5; i++){
        printf("%u\n",next_state_ids_for_black[i]);
        assert(next_state_ids_for_black[i] == expectations[i]);
    }


    // make_graph

    make_graph(graph_table, inv_graph_table,condition_table);

    printf("Saving...\n");

    // save_table(graph_table, "graph_table.dat");
    // save_table(inv_graph_table, "inv_graph_table.dat");
    // save_table(condition_table, "condition_table.dat");
   
    printf("Saved!\n");

    
    // printf("the number of data_items in black_table : %d\n",count_data_items(graph_table));
    // reconstruct_graph_from_file(revived_graph_table,"graph_table.dat");
    // printf("the number of data_items in revived_black_table : %d\n", count_data_items(revived_graph_table));

    
    //is_same_table
    // assert(is_same_table(graph_table,graph_table));
  
    //reconstruct_graph_from_file
    // assert(is_same_table(graph_table,revived_graph_table));


    //edge_num_table
    
    DataItem **edge_num_table = edge_num_count(inv_graph_table);
    printf("edge_num_count finished");


    //cal_best
    calculate_best(graph_table,inv_graph_table,condition_table,edge_num_table);
    printf("calculate_best finished");

    //-----------------------------------------------------------------------

    Move move = {0, 0, 0, 0};

    // 1: first player, 2: second player
    int user_num = atoi(argv[1]) + 1;
    int turn = 1;
    int turn_count = 1;

    
    char input[5];

    
    display_board(board);

    while(1){
        
        if(turn_count > 300){
            printf("Draw");
            break;
        }

        scanf("%s", input);
        parser(input, &move);

        

        if(move_piece(board,move,turn)){

            display_board(board);

            if(judge(board) == turn && user_num == turn){
                printf("You Win");
                break;
            }else if(judge(board) == turn && user_num != turn){
                printf("You Lose");
                break;
            }
            update_turn(&turn,&turn_count);
        }else{
            printf("Invalid move!!\n");
            display_board(board);
        }





    }
    
    
   



    return 0;
}

