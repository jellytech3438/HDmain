#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/datastruct.h"

// print color
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define PLOT_PADDING 2

// print box drawing char
const wchar_t
    VRT1 = L'\x2551', // ║
    VRT2 = L'\x2502', // │
    HRZ1 = L'\x2550', // ═
    HRZ2 = L'\x2500', // ─
    TUP  = L'\x2569', // ╩
    TDWN1 = L'\x2566', // ╦
    TDWN2 = L'\x252C', // ┬
    TDWN3 = L'\x2564', // ╤
    INTR = L'\x256B', // ╫
    LLCNR1 = L'\x255A', //╚
    LLCNR2 = L'\x2514', //└
    LUCNR1 = L'\x2554', // ╔
    LUCNR2 = L'\x250C', // ┌
    RLCNR1 = L'\x255D', // ╝
    RLCNR2 = L'\x2518', // ┘
    RUCNR1 = L'\x2557', //╗
    RUCNR2 = L'\x2510', //┐
    LINTR1 = L'\x2560', // ╠
    LINTR2 = L'\x251C', // ├
    LINTR3 = L'\x255E', // ╞
    LINTR4 = L'\x255F', // ╟
    RINTR1 = L'\x2563', // ╣
    RINTR2 = L'\x2524', // ┤
    RINTR3 = L'\x2561', // ╡
    RINTR4 = L'\x2562'; // ╢

void plot_table(char* t_name){
  printf(WHT"%lc"RESET, LUCNR1);
  for (int _ = 0; _ < strlen(t_name); _++) {
    printf(WHT"%lc"RESET, HRZ1);
  }
  printf(WHT"%lc\n"RESET, RUCNR1);

  printf(WHT"%lc"RESET CYN"%s"RESET WHT"%lc"RESET"\n"RESET,VRT1 ,t_name, VRT1);

  printf(WHT"%lc"RESET, LLCNR1);
  for (int _ = 0; _ < strlen(t_name); _++) {
    if (_ == PLOT_PADDING-1) {
      printf(WHT"%lc"RESET, TDWN1);
    }else{
      printf(WHT"%lc"RESET, HRZ1);
    }
  }
  printf(WHT"%lc\n"RESET, RLCNR1);
}

void plot_column(char* c_name, int t_col_l, int col_l){
  for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
  printf(WHT"%lc "RESET,VRT1);
  printf(WHT"%lc"RESET, LUCNR2);
  int box_width = strlen(c_name) < PLOT_PADDING ? PLOT_PADDING : strlen(c_name);
  for (int _ = 0; _ < box_width; _++) {
    printf(WHT"%lc"RESET, HRZ2);
  }
  printf(WHT"%lc\n"RESET, RUCNR2);

  if (t_col_l == col_l-1) {
    for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
    printf(WHT"%lc%lc%lc"RESET,LLCNR1,HRZ1,RINTR3);
    printf(CYN"%s"RESET ,c_name);
    if (box_width != strlen(c_name)) {
      for (int _ = 0; _ < box_width - strlen(c_name); _++) printf(WHT" "RESET);
    }
    printf(WHT"%lc\n"RESET,VRT2 );

    for (int _ = 0; _ < 2 + PLOT_PADDING; _++) printf(WHT" "RESET);
    printf(WHT"%lc"RESET, LLCNR2);
    for (int _ = 0; _ < box_width; _++) {
      if (_ == PLOT_PADDING-1) {
        printf(WHT"%lc"RESET, TDWN2);
      }else{
        printf(WHT"%lc"RESET, HRZ2);
      }
    }
    printf(WHT"%lc\n"RESET, RLCNR2);
  }else{
    for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
    printf(WHT"%lc%lc%lc"RESET,LINTR1,HRZ1,RINTR3);
    printf(CYN"%s"RESET ,c_name);
    if (box_width != strlen(c_name)) {
      for (int _ = 0; _ < box_width - strlen(c_name); _++) printf(WHT" "RESET);
    }
    printf(WHT"%lc\n"RESET,VRT2 );

    for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
    printf(WHT"%lc "RESET,VRT1);
    printf(WHT"%lc"RESET, LLCNR2);
    for (int _ = 0; _ < box_width; _++) {
      if (_ == PLOT_PADDING-1) {
        printf(WHT"%lc"RESET, TDWN2);
      }else{
        printf(WHT"%lc"RESET, HRZ2);
      }
    }
    printf(WHT"%lc\n"RESET, RLCNR2);
  }
}

void plot_all_data(table* cur_table){
  // print table
  plot_table(cur_table->name);

  column **temp = &cur_table->columns;
  int col_l = 0;
  while(*temp != NULL){
    // print column
    plot_column((*temp)->name, col_l, cur_table->column_len);

    for (int i = 0; i < cur_table->data_len; i++) {
      if ((*temp)->data_type == INT) {
        // for column continue line
        if (col_l == cur_table->column_len-1) {
          for (int _ = 0; _ < 2 * PLOT_PADDING + 2; _++) printf(WHT" "RESET);
        }else{
          for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
          printf(WHT"%lc "RESET,VRT1);
          for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
        }
        // for data
        if (i == cur_table->data_len-1) {
          printf(WHT"%lc%lc"RESET,LLCNR2,HRZ2);
        }else{
          printf(WHT"%lc%lc"RESET,LINTR2,HRZ2);
        }
        printf("%d\n", (*temp)->data.int_data[i]);
      }else if ((*temp)->data_type == STRING) {
        // for column continue line
        if (col_l == cur_table->column_len-1) {
          for (int _ = 0; _ < 2 * PLOT_PADDING + 2; _++) printf(WHT" "RESET);
        }else{
          for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
          printf(WHT"%lc "RESET,VRT1);
          for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
        }
        // for data
        if (i == cur_table->data_len-1) {
          printf(WHT"%lc%lc"RESET,LLCNR2,HRZ2);
        }else{
          printf(WHT"%lc%lc"RESET,LINTR2,HRZ2);
        }
        printf("%s\n", (*temp)->data.string_data[i]);
      }else if ((*temp)->data_type == FLOAT) {
        // for column continue line
        if (col_l == cur_table->column_len-1) {
          for (int _ = 0; _ < 2 * PLOT_PADDING + 2; _++) printf(WHT" "RESET);
        }else{
          for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
          printf(WHT"%lc "RESET,VRT1);
          for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
        }
        // for data
        if (i == cur_table->data_len-1) {
          printf(WHT"%lc%lc"RESET,LLCNR2,HRZ2);
        }else{
          printf(WHT"%lc%lc"RESET,LINTR2,HRZ2);
        }
        printf("%f\n", (*temp)->data.float_data[i]);
      }
    }
    temp = &(*temp)->next;
    col_l++;
  }
}

void plot_some_data(table* cur_table, bool* get_index){

  int first_truth = 0;
  bool first_found = false;
  int last_truth = 0;

  for (int i = 0; i < cur_table->data_len; i++) {
    if (get_index[i]) {
      last_truth = i;
      if (first_found == false) {
        first_truth = i;
        first_found = true;
      }
    }
  }

  // print table
  plot_table(cur_table->name);

  column **temp = &cur_table->columns;
  int col_l = 0;
  while(*temp != NULL){
    // print column
    plot_column((*temp)->name, col_l, cur_table->column_len);

    for (int curindex = first_truth; curindex <= last_truth; curindex++) {
      if (get_index[curindex]) {
        if ((*temp)->data_type == INT) {
          // for column continue line
          if (col_l == cur_table->column_len-1) {
            for (int _ = 0; _ < 2 * PLOT_PADDING + 2; _++) printf(WHT" "RESET);
          }else{
            for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
            printf(WHT"%lc "RESET,VRT1);
            for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
          }
          // for data
          if (curindex == last_truth) {
            printf(WHT"%lc%lc"RESET,LLCNR2,HRZ2);
          }else{
            printf(WHT"%lc%lc"RESET,LINTR2,HRZ2);
          }
          printf(WHT"["RESET YEL"column=%s index=%d"RESET WHT"]"RESET" %d\n",(*temp)->name, curindex, (*temp)->data.int_data[curindex]);
        }else if ((*temp)->data_type == STRING) {
          // for column continue line
          if (col_l == cur_table->column_len-1) {
            for (int _ = 0; _ < 2 * PLOT_PADDING + 2; _++) printf(WHT" "RESET);
          }else{
            for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
            printf(WHT"%lc "RESET,VRT1);
            for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
          }
          // for data
          if (curindex == last_truth) {
            printf(WHT"%lc%lc"RESET,LLCNR2,HRZ2);
          }else{
            printf(WHT"%lc%lc"RESET,LINTR2,HRZ2);
          }
          printf(WHT"["RESET YEL"column=%s index=%d"RESET WHT"]"RESET" %s\n",(*temp)->name, curindex, (*temp)->data.string_data[curindex]);
        }else if ((*temp)->data_type == FLOAT) {
          // for column continue line
          if (col_l == cur_table->column_len-1) {
            for (int _ = 0; _ < 2 * PLOT_PADDING + 2; _++) printf(WHT" "RESET);
          }else{
            for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
            printf(WHT"%lc "RESET,VRT1);
            for (int _ = 0; _ < PLOT_PADDING; _++) printf(WHT" "RESET);
          }
          // for data
          if (curindex == last_truth) {
            printf(WHT"%lc%lc"RESET,LLCNR2,HRZ2);
          }else{
            printf(WHT"%lc%lc"RESET,LINTR2,HRZ2);
          }
          printf(WHT"["RESET YEL"column=%s index=%d"RESET WHT"]"RESET" %f\n",(*temp)->name, curindex, (*temp)->data.float_data[curindex]);
        }
      }
    }
    temp = &(*temp)->next;
    col_l++;
  }
}
