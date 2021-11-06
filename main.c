#include<stdio.h>
#include<stdlib.h>

typedef struct {
    int x0;
    int y0;
    int x1;
    int y1;
} Move;

typedef struct {
    int x;
    int y;
} Vector;


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

    move->x0 = input[1] - 65;
    move->y0 = 5 - ctoi(input[0]);
    move->x1 = input[3] - 65;
    move->y1 = 5 - ctoi(input[2]);

    
}


// 1 : 動かせる, 0: 動かせない
int can_move(int board[5][5], Move move, int turn){
    //現在のターンの人が動かせる駒でなければ動かせない
    if(board[move.y0][move.x0] != turn) return 0;

    //動かす先に駒がすでに存在したら動かせない
    if(board[move.y1][move.x1] != 0) return 0;

    Vector move_vec = {move.x1 - move.x0, move.y1 - move.y0};
    int vec_gcd = gcd(abs(move_vec.x), abs(move_vec.y));

    //成分が互いに素であるベクトル
    Vector coprime_move_vec = { move_vec.x / vec_gcd ,move_vec.y / vec_gcd };

    int front_x = move.x1 + coprime_move_vec.x;
    int front_y = move.y1 + coprime_move_vec.y;

    //縦横斜めのベクトルではなかったら動かせない.( (0,±1),(±1,0),(±1,±1)以外のベクトルの長さの２乗は2より大きくなる)
    if(coprime_move_vec.x * coprime_move_vec.x + coprime_move_vec.y * coprime_move_vec.y > 2) return 0;

    //目の前が壁だったらOK
    if(front_x < 0 || front_x > 4 || front_y < 0 || front_y > 4){
        return 1;
    }else if(board[front_y][front_x] != 0){ // 目の前が駒ならOK
        return 1;
    }else{ //それ以外だったら動かせない
        return 0;
    }  
   
}


int move_piece(int board[5][5], Move move,int turn){
    if(can_move(board,move,turn)){
        int piece = board[move.y0][move.x0];
        board[move.y0][move.x0] = 0;
        board[move.y1][move.x1] = piece;
        return 1;
    }else{
        return 0;
    }
}

void update_turn(int* turn){
    if(*turn == 1){
        *turn = 2;
    }else{
        *turn = 1;
    }
}

int main(int argc,char *argv[]){


    int board[5][5] = {
        {0,2,0,2,0},
        {0,0,1,0,0},
        {0,0,0,0,0},
        {0,0,2,0,0},
        {0,1,0,1,0}
    };

    Move move = {0,0,0,0};

    // 1: first player, 2: second player
    int user_num = atoi(argv[1]) + 1;
    int turn = 1;

    
    char input[5];

    
    display_board(board);

    while(1){
        scanf("%s", input);
        parser(input, &move);

        if(move_piece(board,move,turn)){
            display_board(board);
            update_turn(&turn);
        }else{
            printf("Invalid move!!");
            display_board(board);
        }

    }
    

    
   



    return 0;
}
