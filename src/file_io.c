#include "file_io.h"
#include "hash_table.h"
#include "utility.h"

#include <stdio.h>
#include <stdlib.h>

void save_hash_table(DataItem **table, char *file_path)
{
    FILE *fpw = fopen(file_path, "wb");

    for (int i = 0; i < SIZE; i++)
    {
        if (table[i] == NULL)
            continue;

        recursive_save(table[i], fpw);
    }

    fclose(fpw);
}
void recursive_save(DataItem *data_item, FILE *fpw)
{
    if (data_item->next == NULL)
    {
        fwrite(data_item, sizeof(DataItem), 1, fpw);
    }
    else
    {
        fwrite(data_item, sizeof(DataItem), 1, fpw);
        recursive_save(data_item->next, fpw);
    }
}

void save_int_table(unsigned int table[][DATA_LENGTH], char *file_path)
{
    FILE *fpw = fopen(file_path, "wb");

    fwrite(table, sizeof(int), SIZE * DATA_LENGTH, fpw);

    fclose(fpw);
}

void save_int_array(unsigned int array[], char *file_path)
{
    FILE *fpw = fopen(file_path, "wb");

    fwrite(array, sizeof(int), SIZE, fpw);

    fclose(fpw);
}

void save_short_array(short int array[], char *file_path)
{
    FILE *fpw = fopen(file_path, "wb");

    fwrite(array, sizeof(short int), SIZE, fpw);

    fclose(fpw);
}

void load_hash_table_from_file(DataItem **table, char *file_path)
{
    FILE *fpr = fopen(file_path, "rb");

    DataItem *data_item = (DataItem *)calloc(SIZE, sizeof(DataItem));

    for (int i = 0; i < STATE_NUM; i++)
    {

        fread(&data_item[i], sizeof(DataItem), 1, fpr);
        if (data_item[i].key == 0)
        {
            break;
        }
        hash_insert(table, data_item[i].key, data_item[i].data);
    }

    free(data_item);

    fclose(fpr);
}

void load_int_table_from_file(unsigned int table[][DATA_LENGTH], char *file_path)
{
    FILE *fpr = fopen(file_path, "rb");

    fread(table, sizeof(int), SIZE * DATA_LENGTH, fpr);

    fclose(fpr);
}

void load_int_array_from_file(unsigned int array[], char *file_path)
{
    FILE *fpr = fopen(file_path, "rb");

    fread(array, sizeof(int), SIZE, fpr);

    fclose(fpr);
}

void load_short_array_from_file(short int array[], char *file_path)
{
    FILE *fpr = fopen(file_path, "rb");

    fread(array, sizeof(short int), SIZE, fpr);

    fclose(fpr);
}


