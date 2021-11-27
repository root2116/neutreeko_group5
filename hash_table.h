#ifndef INCLUDED_HASH_TABLE
#define INCLUDED_HASH_TABLE
//------------------------------------------------


//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------

//3540516 * 2 = 7081032を超える最小の素数
#define SIZE 7081049
#define MAX_TRANSITION 32 // 8方位 * 3駒
//------------------------------------------------
//  型定義(Type definition)
//------------------------------------------------
struct DataItem
{
    unsigned int data[MAX_TRANSITION];
    unsigned int key;
    struct DataItem *next;
};
typedef struct DataItem DataItem;

//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------

void hash_init(DataItem **table);

// static void cell_free(DataItem *cell);

int get_hash_value(unsigned int key);

unsigned int* hash_search(DataItem **talbe, unsigned int key);

int hash_insert(DataItem **table, unsigned int key, unsigned int *data);

// int hash_delete(DataItem **table, int key);

void hash_append_data(DataItem **table,unsigned int key, unsigned int new_id);
//------------------------------------------------
#endif