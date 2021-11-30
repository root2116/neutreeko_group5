#include "utility.h"
#include "hash_table.h"

#include <stdio.h>


void init_table(unsigned int graph_table[SIZE][DATA_LENGTH])
{
    int i, j;
    for (i = 0; i < SIZE; i += 1)
    {
        for (j = 0; j < DATA_LENGTH; j += 1)
        {
            graph_table[i][j] = 0;
        }
    }
}

void show_graph(DataItem **dictionary, unsigned int graph_table[SIZE][DATA_LENGTH], int c)
{
    int i, j;
    printf("show graph below\n");
    for (i = 0; i < c; i += 1)
    {
        for (j = 0; j < DATA_LENGTH; j += 1)
        {
            printf("%u ", graph_table[i][j]);
        }
        printf("\n");
    }
}

void generate_board_from_array(int board[5][5], int **board_num_array)
{

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            board[i][j] = 0;
        }
    }

    int x, y;
    for (int i = 0; i < 6; i++)
    {
        x = *board_num_array[i] % 5;
        y = *board_num_array[i] / 5;
        if (i < 3)
        {
            board[y][x] = WHITE;
        }
        else
        {
            board[y][x] = BLACK;
        }
    }
}

int get_color_from_state_id(unsigned int state_id)
{
    int turn = state_id >> 31;
    return turn + 1;
}

unsigned int encode_board(int board[][5], int turn)
{
    int komacount = 0;
    unsigned int board_id;
    int sub_id1 = 0;
    int sub_id2 = 0;

    /*盤面に対して別々にエンコード*/
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (board[i][j] == 1)
            {
                sub_id1 += 1 << (24 - (5 * i + j));
                komacount++;
            }
            else if (board[i][j] == 2)
            {
                sub_id1 += 1 << (24 - (5 * i + j));
                sub_id2 += 1 << (5 - komacount);
                komacount++;
            }
            else
            {
            }
        }
    }

    board_id = ((turn - 1) << 31) + sub_id1 * 64 + sub_id2;
    return board_id;
}

void decode_state_id(unsigned int board_id, int board[][5])
{
    /*sub_id2が黒白の順番、sub_id1が駒のあるなし*/
    int sub_id2 = board_id % 64;
    int sub_id1 = board_id / 64;

    for (int i = 24; i >= 0; i--)
    {
        if ((sub_id1 % 2) == 1)
        {
            if ((sub_id2 % 2) == 0)
            {
                board[(i / 5)][(i % 5)] = 1;
            }
            else
            {
                board[(i / 5)][(i % 5)] = 2;
            }
            sub_id2 /= 2;
        }
        else
        {
            board[(i / 5)][(i % 5)] = 0;
        }
        sub_id1 /= 2;
    }
    return;
}
