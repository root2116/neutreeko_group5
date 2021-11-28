#ifndef INCLUDED_QUEUE
#define INCLUDED_QUEUE
//------------------------------------------------
#include "hash_table.h"
#include "main.h"
#include <stdio.h>
//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------
#define MAX_QUEUE_LENGTH 2600000 //最大値は2592964らしいので

//------------------------------------------------
//  型定義(Type definition)
//------------------------------------------------
typedef struct Queue
{
    /* データの最後尾 */
    int tail;
    /* データの先頭 */
    int head;
    /* キューされているデータ */
    unsigned int data[MAX_QUEUE_LENGTH];
} Queue;

//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------

void queue_init(Queue *);
int queue_empty(Queue *);
int queue_length(Queue *);
void enqueue(Queue *, unsigned int);
unsigned int dequeue(Queue *);

//------------------------------------------------
#endif