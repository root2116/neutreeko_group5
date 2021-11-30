

#include "game.h"
#include "utility.h"
#include "hash_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int gcd(int x, int y)
{

    if (y == 0)
    {
        return x;
    }
    else
    {
        return gcd(y, x % y);
    }
}
//char to int
int ctoi(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    return 0;
}

void parser(char *input, Move *move)
{

    move->start.x = input[1] - 65;
    move->start.y = 5 - ctoi(input[0]);
    move->end.x = input[3] - 65;
    move->end.y = 5 - ctoi(input[2]);
}

//２つの点a,bからベクトルabを作る
void make_vec(Point *a, Point *b, Vector *v)
{
    v->x = b->x - a->x;
    v->y = b->y - a->y;
}

//点にベクトルを足した点を返す
void add_vec_to_point(Point *a, Vector *v, Point *b)
{
    b->x = a->x + v->x;
    b->y = a->y + v->y;
}

/*ゲームが終わっているか判定する関数*/

int judge_one_side(int board[5][5], int color)
{
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

int judge(int board[5][5])
{
    int result = judge_one_side(board, BLACK);

    if (result != 0)
    {
        return 1;
    }

    result = judge_one_side(board, WHITE);

    if (result != 0)
    {
        return 2;
    }

    return 0;
}

int is_on_wall(Point p)
{
    if (p.x == -1 || p.x == 5 || p.y == -1 || p.y == 5)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_out_board(Point p)
{
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
int can_move(int board[5][5], Move move, int turn)
{
    //現在のターンの人が動かせる駒でなければ動かせない
    if (board[move.start.y][move.start.x] != turn)
        return 0;

    //動かす先に駒がすでに存在したら動かせない
    if (board[move.end.y][move.end.x] != EMPTY)
        return 0;

    Vector move_vec;
    make_vec(&move.start, &move.end, &move_vec);

    int vec_gcd = gcd(abs(move_vec.x), abs(move_vec.y));

    //成分が互いに素であるベクトル
    Vector coprime_move_vec = {move_vec.x / vec_gcd, move_vec.y / vec_gcd};

    //縦横斜めのベクトルではなかったら動かせない.( (0,±1),(±1,0),(±1,±1)以外のベクトルの長さの２乗は2より大きくなる)
    if (coprime_move_vec.x * coprime_move_vec.x + coprime_move_vec.y * coprime_move_vec.y > 2)
        return 0;

    Point front;
    add_vec_to_point(&move.end, &coprime_move_vec, &front);

    Point scan;
    add_vec_to_point(&move.start, &coprime_move_vec, &scan);

    //射線上に駒がいたら動かせない
    while (scan.x != move.end.x || scan.y != move.end.y)
    {
        if (board[scan.y][scan.x] != EMPTY)
            return 0;

        add_vec_to_point(&scan, &coprime_move_vec, &scan);
    }

    //目の前が壁だったらOK
    if (is_on_wall(front))
    {
        return 1;
    }
    else if (is_out_board(front))
    {
        return 0;
    }
    else if (board[front.y][front.x] != EMPTY)
    { // 目の前が駒ならOK
        return 1;
    }
    else
    { //それ以外だったら動かせない
        return 0;
    }
}

int move_piece(int board[5][5], Move move, int turn)
{
    if (can_move(board, move, turn))
    {
        int piece = board[move.start.y][move.start.x];
        board[move.start.y][move.start.x] = EMPTY;
        board[move.end.y][move.end.x] = piece;
        return 1;
    }
    else
    {
        return 0;
    }
}

void update_turn(int *turn, int *turn_count)
{
    *turn_count = *turn_count + 1;
    if (*turn == 1)
    {
        *turn = 2;
    }
    else
    {
        *turn = 1;
    }
}

void display_board(int board[5][5])
{
    //読み込み
    int width = 6;
    char boardmap[width][width];

    boardmap[0][0] = ' ';

    for (int i = 1; i < width; i++)
    {
        boardmap[0][i] = 'A' + i - 1;
        boardmap[i][0] = '6' - i;
    }

    //新しいマップ
    for (int i = 1; i < width; i++)
    {
        for (int j = 1; j < width; j++)
        {
            if (board[i - 1][j - 1] == BLACK)
            {
                boardmap[i][j] = 'O';
            }
            else if (board[i - 1][j - 1] == WHITE)
            {
                boardmap[i][j] = '@';
            }
            else
            {
                boardmap[i][j] = ' ';
            }
        }
    }

    //プリント

    printf("\n");

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {
            printf("%c|", boardmap[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}


void start_game(int init_turn, DataItem **dict,short int best_move_array[]){

    int board[5][5] = {
        {0, 2, 0, 2, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 2, 0, 0},
        {0, 1, 0, 1, 0}};

    Move move = {0, 0, 0, 0};

    // 1: first player, 2: second player
    int user_num = init_turn + 1;
    int turn = 1;
    int turn_count = 1;

    char input[5];

    display_board(board);

    while (1)
    {

        if (turn_count > 300)
        {
            printf("Draw\n");
            break;
        }
        if(turn == user_num){
            scanf("%s", input);
        }else{
            game_ai(dict,best_move_array,board,turn,input);
            printf("%s\n",input);
        }

        
        parser(input, &move);

        if (move_piece(board, move, turn))
        {

            display_board(board);

            if (judge(board) == turn && user_num == turn)
            {
                printf("You Win\n");
                break;
            }
            else if (judge(board) == turn && user_num != turn)
            {
                printf("You Lose\n");
                break;
            }
            update_turn(&turn, &turn_count);
        }
        else
        {
            printf("Invalid move!!\n");
            display_board(board);
        }
    }
}

void game_ai(DataItem **dict,short int best_move_array[], int board[][5], int turn, char* best_move){

    

    int state_id = encode_board(board,turn);

    int state_index = hash_search(dict,state_id);

    short int best_move_int = best_move_array[state_index];

    short int tmp = best_move_int;


    
    convert_move_int_into_move_string(best_move_int,best_move);

  


}


void convert_move_int_into_move_string(short int move_int, char move[4]){
    short int tmp = move_int;

    move[3] = 'A' + tmp % 10;
    tmp /= 10;
    move[2] = '0' + tmp % 10;
    tmp /= 10;
    move[1] = 'A' + tmp % 10;
    tmp /= 10;
    move[0] = '0' + tmp % 10;



}   