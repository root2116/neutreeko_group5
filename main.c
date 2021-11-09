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



/*ゲームが終わっているか判定する関数*/

int judge (int board[5][5]){

    /*黒をチェック*/
    int n = 1;  /*コマの色*/
    int out = 1;  /*返り値*/
    
    /*黒のコマがある場所を確認　（2次元配列を1次元に落としてます）*/
    int tmp[3];
    int c = 0;
    int i, j;
    
    for (i = 0; i < 5; i += 1){
        for (j = 0; j < 5; j += 1){
            if (board[i][j] == n){
                tmp[c] = 5*i + j;
                c += 1;
            }
        }
    }

    int sa1, sa2;
    sa1 = tmp[1] - tmp[0];
    sa2 = tmp[2] - tmp[1];

    if (sa1 == sa2){
        if (sa1 == 1){/*横*/
            if (tmp[0]/5 == tmp[1]/5 && tmp[1]/5 == tmp[2]/5){
                return out;
            }
        }
        if (sa1 == 5){/*縦*/
            return out;
        }
        if (sa1 == 6){/*斜め　左上->右下*/
            if (tmp[0]/5 + 1 == tmp[1]/5 && tmp[1]/5 + 1 == tmp[2]/5){
                return out;
            }
        }
        if (sa1 == 4){/*斜め　右上->左下*/
            if (tmp[0]/5 + 1 == tmp[1]/5 && tmp[1]/5 + 1 == tmp[2]/5){
                return out;
            }
        }
    }

    /*  白をチェック*/
    n = 2;
    out = 2;


    c = 0;

    
    for (i = 0; i < 5; i += 1){
        for (j = 0; j < 5; j += 1){
            if (board[i][j] == n){
                tmp[c] = 5*i + j;
                c += 1;
            }
        }
    }


    sa1 = tmp[1] - tmp[0];
    sa2 = tmp[2] - tmp[1];

    if (sa1 == sa2){
        if (sa1 == 1){/*横*/
            if (tmp[0]/5 == tmp[1]/5 && tmp[1]/5 == tmp[2]/5){
                return out;
            }
        }
        if (sa1 == 5){/*縦*/
            return out;
        }
        if (sa1 == 6){/*斜め　左上->右下*/
            if (tmp[0]/5 + 1 == tmp[1]/5 && tmp[1]/5 + 1 == tmp[2]/5){
                return out;
            }
        }
        if (sa1 == 4){/*斜め　右上->左下*/
            if (tmp[0]/5 + 1 == tmp[1]/5 && tmp[1]/5 + 1 == tmp[2]/5){
                return out;
            }
        }
    
    }
    /*ゲーム継続*/
    out = 0;
    return out;
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
        

    
    //縦横斜めのベクトルではなかったら動かせない.( (0,±1),(±1,0),(±1,±1)以外のベクトルの長さの２乗は2より大きくなる)
    if(coprime_move_vec.x * coprime_move_vec.x + coprime_move_vec.y * coprime_move_vec.y > 2) return 0;

    int front_x = move.x1 + coprime_move_vec.x;
    int front_y = move.y1 + coprime_move_vec.y;

    int scan_x = move.x0 + coprime_move_vec.x;
    int scan_y = move.y0 + coprime_move_vec.y;

    //射線上に駒がいたら動かせない
    while(scan_x != move.x1 || scan_y != move.y1){
        if(board[scan_y][scan_x] != 0) return 0;

        scan_x += coprime_move_vec.x;
        scan_y += coprime_move_vec.y;
    }
    

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
      if (board[i - 1][j - 1] == 1) {
        boardmap[i][j] = 'O';
      }else if(board[i - 1][j - 1] == 2){
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

