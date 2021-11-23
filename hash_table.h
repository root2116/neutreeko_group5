#ifndef INCLUDED_HASH_TABLE
#define INCLUDED_HASH_TABLE
//------------------------------------------------


//------------------------------------------------
//  マクロ定義(Macro definition)
//------------------------------------------------

//3540516を超える最小の素数
#define SIZE 3540521
#define MAX_TRANSITION 24 // 8方位 * 3駒
//------------------------------------------------
//  型定義(Type definition)
//------------------------------------------------
struct DataItem
{
    int *data;
    int key;
    struct DataItem *next;
};
typedef struct DataItem DataItem;

//------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//------------------------------------------------

void hash_init(DataItem **table);

// static void cell_free(DataItem *cell);

int get_hash_value(int key);

int* hash_search(DataItem **talbe, int key);

int hash_insert(DataItem **table, int key, int *data);

// int hash_delete(DataItem **table, int key);

void hash_append_data(DataItem **table,int key, int new_id);
//------------------------------------------------
#endif