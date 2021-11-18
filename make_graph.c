
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#define SIZE 3542009


struct DataItem {
    long int *data;
    long int key;
    struct DataItem *next;
};
typedef struct DataItem DataItem;

void hash_init(DataItem **table){
    for (int i = 0; i < SIZE; i++){
        table[i] = NULL;
    }
    return;
}

int get_hash_value(long int key){
    return key % SIZE;
}

long int* hash_search(DataItem **table, long int key){
    int hashval = get_hash_value(key);
    DataItem *target = table[hashval];

    for(; target != NULL; target = target->next){
        if(target->key == key){
            return target->data;
        }
    }

    return NULL;
}



int hash_insert(DataItem **table, long int key, long int *data){
    DataItem *p = NULL;
    int hashval = 0;

    if (hash_search(table, key) != NULL){
        fprintf(stderr, "key[%ld] is already exist in hash table.\n", key);
        return (-1);
    }

    p = malloc(sizeof(DataItem));

    if(p == NULL){
        fprintf(stderr, "ERROR: %s(%d line)\n", strerror(errno), __LINE__);
        return(-1);
    }
    p->key = key;
    p->data = data;

    
    hashval = get_hash_value(key);
    p->next = table[hashval];
    table[hashval] = p;

    return 0;

}

int encode_board(int board[][5]) {
  int komacount = 0;
  int key;
  int subkey1 = 0;
  int subkey2 = 0;
  
  /*盤面に対して別々にエンコード*/
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      if (board[i][j] == 1) {
        subkey1 += 2 ** (24 - (5 * i + j));
        komacount++;
      }
      else if (board[i][j] == 2) {
        subkey1 += 2 ** (24 - (5 * i + j));
        subkey2 += 2 ** (5 - komacount);
        komacount++;
      } else {}
    }
  }
  
  key = subkey1 * (2 ** 6) + subkey2;
  return key;
}

void decode_key(int key, int board[][5]) {
  /*subkey2が黒白の順番、subkey1が駒のあるなし*/
  int subkey2 = key % 64;
  int subkey1 = key / 64;

  for (int i = 24; i >= 0; i--) {
    if ((subkey1 % 2) == 1) {
      if ((subkey2 % 2) == 0) {
        board[(i / 5)][(i % 5)] = 1;
      } else {
        board[(i / 5)][(i % 5)] = 2;
      }
      subkey2 /= 2;
    } else {}
    subkey1 /= 2;
  }
  return;
}

DataItem *table[SIZE];

int main(){
    

    hash_init(table);

    long int moves[] = {20418082124,20418082324,20418070824,20418081624,20418082223,20418082225,20418082022,20418080922,
    20418062224,20418102224,20418032224,20418132224,20418102224,20418202224};
    hash_insert(table, 20418082224, moves);

    printf("%ld\n", hash_search(table, 20418082224)[0]);

    int board[5][5] = {
        {0, 2, 0, 2, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 2, 0, 0},
        {0, 1, 0, 1, 0}
        };

    printf("%ld\n",encode_board(board));

    return 0;
}