#include "hash_table.h"
#include "make_graph.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>




void hash_init(DataItem **table)
{
    for (int i = 0; i < SIZE; i++)
    {
        table[i] = NULL;
    }
    return;
}



int get_hash_value(unsigned int key)
{
    return key % SIZE;
}

unsigned int *hash_search(DataItem **table, unsigned int key)
{
    int hashval = get_hash_value(key);
    DataItem *target = table[hashval];

    for (; target != NULL; target = target->next)
    {
        if (target->key == key)
        {
            return target->data;
        }
    }

    return NULL;
}

int hash_insert(DataItem **table, unsigned int key, unsigned int *data)
{
    DataItem *p = NULL;
    int hashval = 0;

    if (hash_search(table, key) != NULL)
    {
        fprintf(stderr, "key[%u] already exists in hash table.\n", key);
        return (-1);
    }

    p = malloc(sizeof(DataItem));

    if (p == NULL)
    {
        fprintf(stderr, "ERROR: %s(%d line)\n", strerror(errno), __LINE__);
        return (-1);
    }
    p->key = key;

    for (int i = 0; i < DATA_LENGTH; i++){
        p->data[i] = data[i];
    }

    hashval = get_hash_value(key);
    p->next = table[hashval];
    table[hashval] = p;

    return 0;
}

int hash_delete(DataItem **table, unsigned int key)
{
    DataItem *target = NULL;
    DataItem *chain = NULL;

    int hashval = get_hash_value(key);

    /* ハッシュキーがハッシュテーブルに存在しているか確認する */
    target = table[hashval];
    if (target == NULL)
    {
        fprintf(stderr, "target[%u] is not exist in hash table.\n", key);
        return (-1);
    }
    chain = target->next;

    /* リストの先頭要素を削除する場合 */
    if (target->key == key)
    {
        table[hashval] = chain;
        free(target);
        return (0);
    }

    /* 先頭以外の要素を削除する場合 */
    while (target != NULL)
    {
        if (target->key == key)
        {
            chain->next = target->next;
            free(target);
            return (0);
        }

        chain = target;
        target = target->next;
    }

    return (-1);
}

void hash_append_data(DataItem **table, unsigned int key, unsigned int new_id){
    unsigned int *data = hash_search(table,key);
    if(data == NULL){
        unsigned int *new_data = malloc(sizeof(int) * DATA_LENGTH);

        for(int i = 0; i < DATA_LENGTH; i++){
            new_data[i] = END;
        }
        new_data[0] = new_id;
        hash_insert(table, key, new_data);
    }else{
        for(int i = 0; i < DATA_LENGTH; i++){
            if(data[i] == END){
                data[i] = new_id;
                break;
        }
    }
    }
   
}