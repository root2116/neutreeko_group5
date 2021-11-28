#include<stdio.h>
#include "hash_table.h"

// int is_same_board(int array1[5][5], int array2[5][5]){
    
//     for(int i = 0; i < 5; i++){
//         for(int j = 0; j < 5; j++){
//             if(array1[i][j] != array2[i][j]){
//                 return 0;
//             }
//         }
//     }

//     return 1;
// }

// int is_same_array(unsigned int *array1, unsigned int *array2, int len)
// {
//     for (int i = 0; i < len; i++)
//     {
//         if (array1[i] != array2[i])
//         {
//             return 0;
//         }
//     }

//     return 1;
// }

// int depth_of_data_item_list(DataItem *data_item)
// {
//     int count = 0;

//     DataItem *cur = data_item;
//     while (1)
//     {

//         if (cur->next == NULL)
//         {
//             return count;
//         }

//         cur = cur->next;
//         count++;
//     }
// }

// int is_same_table(DataItem **table1,DataItem **table2){
    
//     for(int i = 0; i < SIZE; i++){
    
        
//         if(table1[i] == NULL && table2[i] == NULL) continue;
//         else if(table1[i] != NULL && table2[i] != NULL){
//             DataItem *cur = table1[i];
//             while(1){
//                 // table1[i] ⊂ table2[i]であることを確認する
//                 unsigned int *correspondence = hash_search(table2,cur->key);

//                 //そもそも対応物がない場合
//                 if(correspondence == NULL) return 0;

//                 //keyは同じだが中身が違う場合
//                 if(is_same_array(cur->data,correspondence,DATA_LENGTH) != 1) return 0;

                
//                 //table1[i] ⊂ table2[i]が確認できた上で、要素数が同じなら, table1[i] は table2[i] に等しい
//                 if(cur->next == NULL){
//                     if(depth_of_data_item_list(table1[i]) == depth_of_data_item_list(table2[i])){
//                         break;
//                     }
//                 }else{
//                     cur = cur->next;
//                 }

//             }
            
//         }else{//片方にデータがあり、もう片方にない場合
//             return 0;
//         }
//     }

//     return 1;
// }

// int count_data_items(DataItem **table){
//     int count = 0;
//     for(int i = 0; i < SIZE; i++){
//         if(table[i] == NULL) continue;

//         count += depth_of_data_item_list(table[i]) + 1;
//     }

//     return count;
// }

// void compare_table_keys(DataItem **table1, DataItem **table2){
//     for (int i = 0; i < SIZE; i++)
//     {

//         if (table1[i] == NULL && table2[i] == NULL)
//             continue;
//         else if (table1[i] != NULL && table2[i] != NULL)
//         {
//             DataItem *cur = table1[i];
//             while (1)
//             {
            
//                 unsigned int *correspondence = hash_search(table2, cur->key);

//                 //そもそも対応物がない場合
//                 if (correspondence == NULL){
//                     printf("table1: %u\n",cur->key);
//                     return;
//                 }
                    


                
//                 if (cur->next == NULL)
//                 {
//                     if (depth_of_data_item_list(table1[i]) == depth_of_data_item_list(table2[i]))
//                     {
//                         break;
//                     }
//                 }
//                 else
//                 {
//                     cur = cur->next;
//                 }
//             }
//         }
//         else
//         { //片方にデータがあり、もう片方にない場合
//             return;
//         }
//     }

    
// }
