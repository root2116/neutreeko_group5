
int* calculate_best(int cond[], int g1[][24], int mate[], int best_table[]){
    int array_length = 3540516;

    
    int i = 0;
    int best = 0;
    int tmp_mate = -array_length;
    int j = 0;
    int tmp_array_length = 24;
    for (i = 0; i < array_length; i += 1){

        /*勝ち盤面なら遷移先の（相手目線の）負け盤面で決着手数の短いものに遷移*/
        if (cond[i] == 1){
            best = 0;
            tmp_mate = -array_length;
            

            for (j = 0; j < tmp_array_length; j += 1){
                if(g1[i][j] == 0){
                    break;
                }
                if (cond[g1[i][j]] == -1){
                    if (mate[g1[i][j]] > tmp_mate){
                        tmp_mate = mate[g1[i][j]];
                        best = g1[i][j];
                    }
                }   
            }
            best_table[i] = best;
        }
        /*負け盤面なら遷移先の（相手目線の）勝ち盤面で決着手数の長いものに遷移*/
        else if (cond[i] == -1){
            best = 0;
            tmp_mate = -array_length;
            

            for (j = 0; j < tmp_array_length; j += 1){
                if(g1[i][j] == 0){
                    break;
                }
                if (cond[g1[i][j]] == 1){
                    if (mate[g1[i][j]] > tmp_mate){
                        tmp_mate = mate[g1[i][j]];
                        best = g1[i][j];
                    }
                }else{
                    printf("err51");
                }  
            }
            best_table[i] = best;
        }
        /*引き分け盤面なら遷移先の（相手目線の）引き分け盤面に適当に遷移*/
        else{
            best = 0;
            tmp_mate = -array_length;
            

            for (j = 0; j < tmp_array_length; j += 1){
                if(g1[i][j] == 0){
                    break;
                }
                if (cond[g1[i][j]] == 0){
                    tmp_mate = mate[g1[i][j]];
                    best = g1[i][j];
                    break;    
                }
            }
            best_table[i] = best;
        }
    }
    return &best_table[0];
}
