#include "queue.h"
#include <stdio.h>

void queue_init(Queue *queue){
    queue->head = 0;
    queue->tail = -1;
}

int queue_empty(Queue *queue){
    if ((queue->tail + 1) % MAX_QUEUE_LENGTH == queue->head){
        return 1;
    }else{
        return 0;
    }
}

int queue_length(Queue *queue){
    if (queue->tail < queue->head)
    {
        return queue->tail + MAX_QUEUE_LENGTH - queue->head + 1;
    }
    else
    {
        return queue->tail - queue->head + 1;
    }
}


void enqueue(Queue *queue, unsigned int input){
    /* キューが満杯なら何もせず関数終了 */
    if ((queue->tail + 2) % MAX_QUEUE_LENGTH == queue->head)
    {
        printf("The queue is filled with data. Failed to enqueue.\n");
        return;
    }

    /* データをデータの最後尾の１つ後ろに格納 */
    queue->data[(queue->tail + 1) % MAX_QUEUE_LENGTH] = input;

    /* データの最後尾を１つ後ろに移動 */
    queue->tail = (queue->tail + 1) % MAX_QUEUE_LENGTH;
}
unsigned int dequeue(Queue *queue){
    unsigned int ret = 0;

    /* キューが空なら何もせずに関数終了 */
    if ((queue->tail + 1) % MAX_QUEUE_LENGTH == queue->head)
    {
        printf("The queue is empty\n");
        return -1;
    }

    /* データの先頭からデータを取得 */
    ret = queue->data[queue->head];

    /* データの先頭を１つ後ろにずらす */
    queue->head = (queue->head + 1) % MAX_QUEUE_LENGTH;

    /* 取得したデータを返却 */
    return ret;
}