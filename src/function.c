#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <wchar.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
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

/*
 * below are file io functions
 */

void read_from_file1(table* cur_table, FILE* opentable){
  if (!fread(cur_table,sizeof(struct table),1,opentable)) {
    printf("open table error\n");
    exit(0);
  }

  size_t length;

  fread(&length, sizeof(length), 1, opentable);
  cur_table->name = (char*)malloc(sizeof(char) * length);
  cur_table->columns = (column*)malloc(sizeof(column));

  if (!fread(cur_table->name, 1, length, opentable)) {
    printf("open table error\n");
    exit(0);
  }

  column *temp = (column*)malloc(sizeof(column));
  column *last = cur_table->columns;

  int i = 0;
  while(fread(temp,sizeof(column),1,opentable)){
    if (i != 0) {
      last->next = (column*)malloc(sizeof(column));
      last = last->next;
    }
    strcpy(last->name, temp->name);
    last->data_type = temp->data_type;
    last->data = temp->data;
    i++;
  }
}

void write_to_file1(table* newtable){
  FILE* opentable = fopen(newtable->name, "w");
  size_t length;

  if (fwrite(newtable, sizeof(struct table), 1, opentable) < 0){
   printf("%d\n", errno);
  }

  length = strlen(newtable->name) + 1;
  fwrite(&length, sizeof(length), 1, opentable);
  fwrite(newtable->name, 1, length, opentable);

  column *col = newtable->columns;
  while (col != NULL) {
   if(fwrite(col, sizeof(column), 1, opentable) < 0){
     printf("%d\n", errno);
   }
   col = col->next;
  }
}

/*
 * below are uio version of file io (still in experiments)
 */

void read_from_file2(char* tablename, table* buf, char* t_name, column* columns){
  int opentable = open(tablename, O_RDONLY);
  if(opentable < 0){
    printf("open table error\n");
    exit(0);
  }

  // why do we need this??????
  char buf1[1];

  int iovind = 1;

  struct iovec iov[3];

  iov[0].iov_base = buf;
  iov[0].iov_len = sizeof(table);

  for (int i = 0; i < 3; i++, iovind++) {
    iov[iovind].iov_base = columns+i;
    iov[iovind].iov_len = sizeof(column);
  }

  readv(opentable, iov, iovind);

  // printf("buf1: %s", buf1);
  printf("data_len: %d\n", buf->data_len);

  printf("name: %s\n", (columns+0)->name);
  for (int i = 0; i < buf->data_len; i++) {
    printf("%d\n", (columns+1)->data.int_data[i]);
  }

  printf("read end\n");
}

void write_to_file2(table* newtable){
  int opentable = open(newtable->name, O_WRONLY | O_CREAT);
  if(opentable < 0){
    printf("open table error\n");
    exit(0);
  }
  chmod(newtable->name,0755);
  struct iovec *iovecs = (struct iovec*)malloc(sizeof(table) + newtable->column_len * sizeof(column));

  iovecs[0].iov_base = newtable;
  iovecs[0].iov_len = sizeof(table);

  int io_index = 1;

  column *col = newtable->columns;
  while (col != NULL) {
    iovecs[io_index].iov_base = col;
    iovecs[io_index].iov_len = sizeof(column);
    io_index++;
    col = col->next;
  }

  writev(opentable, iovecs, io_index);
  printf("write end\n");
}

/*
 * below are some basic check and operation functions
 */

void split_column_value1(char* arg, char* column, char* value, char split_symbol){
  char* split;
  if ((split = strchr(arg, split_symbol)) == NULL) {
    printf("unacceptable no type column\n");
    exit(0);
  }

  int arg_index = 0;

  for (; arg[arg_index] != *split; arg_index++) {
    if (isalnum(arg[arg_index]) || arg[arg_index] == '_' || arg[arg_index] == '/') {
      column[arg_index] = arg[arg_index];
    }else{
      printf("unacceptable symbol in column name\n");
      exit(0);
    }
  }

  arg_index++; // jump off the split

  for (int v_index = 0;arg[arg_index] != 0; arg_index++,v_index++) {
    value[v_index] = arg[arg_index];
  }
}

void check_hidden_folder(){
  if (opendir(".hdb") < 0) {
    printf("error database");
    exit(0);
  }
  if (chdir(".hdb") < 0) {
    printf("error open database\n");
    exit(0);
  }
}

bool file_exist(char* path){
  struct stat buffer;
  return (stat(path, &buffer) == 0);
}

bool dir_exist(char* path){
  struct stat buffer;
  if (!stat(path,&buffer)) {
    if (S_ISDIR(buffer.st_mode)) {
      return true;
    }else {
      return false;
    }
  }else{
    return false;
  }
}

choosen check_yn(char* input){
  bool onlyspace = true;
  choosen _choos;
  for (char* ch = input; *ch != '\0'; ch++) {
    if (isspace(*ch)) {
      continue;
    }
    onlyspace = false;
    if (tolower(*ch) != 'y' && tolower(*ch) != 'n') {
      printf("\ninvalid input\n");
      return 3;
    }
    if (tolower(*ch) == 'y') {
      _choos = YES;
    }else if(tolower(*ch) == 'n'){
      _choos = NO;
    }
  }
  if (onlyspace) {
    _choos = DEFAULT;
  }
  return _choos;
}

bool Xor(bool a, bool b){
  return (a + b) % 2;
}

/*
 * below are the datastructure functions
 */

void add_column(table* t, column* c){
  if (t->column_len == 0) {
    t->columns = c;
  }else{
    column **temp = &t->columns;
    while(*temp != NULL){
      if (strcmp((*temp)->name, c->name) == 0) {
        printf("column name already exist\n");
        exit(0);
      }
      temp = &(*temp)->next;
    }
    *temp = c;
  }
  t->column_len++;
}

void delet_column(table* t, char* c_name){
  if (t->column_len == 0) {
    return;
  }else if (t->column_len == 1){
    t->columns = NULL;
    t->column_len--;
    return;
  }else{
    column **pre = &t->columns;
    column **cur = &t->columns;
    int i = 0;
    while(*cur != NULL){
      if (strcmp((*cur)->name, c_name) == 0) {
        if (i == t->column_len-1) {
          (*pre)->next = NULL;
        }else if (pre == cur) {
          if ((*pre)->next == NULL) {
            t->columns = NULL;
          }else{
            t->columns = (*pre)->next;
          }
        }else{
          (*pre)->next = (*cur)->next;
        }
        t->column_len--;
        return;
      }
      if (i != 0) {
        pre = &(*pre)->next;
      }
      cur = &(*cur)->next;
      i++;
    }
    return;
  }
}

column** find_column_by_name(table* t, char* c_name){
  if (t->column_len == 0) {
    return 0;
  }else{
    column **pre = &t->columns;
    column **cur = &t->columns;
    int i = 0;
    while(*cur != NULL){
      if (strcmp((*cur)->name, c_name) == 0) {
        return cur;
      }
      if (i != 0) {
        pre = &(*pre)->next;
      }
      cur = &(*cur)->next;
      i++;
    }
  }
}

/*
 * below are the command line function
 */

void init_database(char* pathname){

  chdir(pathname);

  // use .hdb folder for database store
  if (mkdir(".hdb", 0755) < 0) {
    printf("init error");
    exit(0);
  }

  DIR* dir;

  if ((dir = opendir(".hdb")) == NULL) {
    printf("opendir error");
  }else{
    int table_history;
    if ((table_history = open("table_history", O_WRONLY | O_CREAT | O_APPEND)) < 0) {
      printf("init error");
    }
    chmod("table_history", 0755);
  }
}

void delete_data(table* cur_table, int index){
  column **temp = &cur_table->columns;
  while(*temp != NULL){
    if ((*temp)->data_type == INT) {
      (*temp)->data.int_data[index] = 0;
    }else if ((*temp)->data_type == STRING) {
      strcpy((*temp)->data.string_data[index], "\0");
    }else if ((*temp)->data_type == FLOAT) {
      (*temp)->data.float_data[index] = 0.;
    }
    temp = &(*temp)->next;
  }
}

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

void where_tag(table* t, bool* get_index,logicoperation lo, bool with_not, char* input) {
  // dealing with spliting input
  column **target_col;
  char column[MAXCHARSIZE] = {0};
  char value[MAXCHARSIZE] = {0};

  split_column_value1(input,column,value,'=');

  // index as condition
  if (strcmp(column,"INDEX") == 0) {
    int index = atoi(value);
    if (index >= 0 && index < t->data_len) {
      if (lo == AND) {
        if (get_index[index] == false) {
          if (with_not) {
            // do nothing
          }else{
            memset(get_index, false, t->data_len * sizeof(bool));
          }
        }else{
          if (with_not) {
            get_index[index] = false;
          }else{
            memset(get_index, false, t->data_len * sizeof(bool));
            get_index[index] = true;
          }
        }
      }else if (lo == OR) {
        if (get_index[index] == false) {
          if (with_not) {
            memset(get_index, true, t->data_len * sizeof(bool));
            get_index[index] = false;
          }else{
            get_index[index] = true;
          }
        }else{
          if (with_not) {
            memset(get_index, true, t->data_len * sizeof(bool));
          }else{
            // do nothing
          }
        }
      }else{
        if (with_not) {
          memset(get_index, true, t->data_len * sizeof(bool));
          get_index[index] = false;
        }else{
          get_index[index] = true;
        }
      }
    }
    return;
  }

  // column name and value
  target_col = find_column_by_name(t,column);

  // dealing with index list
  if (lo == AND) {
    for (int j = 0; j < t->data_len; j++) {
      // true since AND
      if (get_index[j] == true){
        /* | xor gate: (with not) ^ (data equal)  |
         * | with not | data equal | get_index[j] |
         * | F        | T          | T            |
         * | F        | F          | F            |
         * | T        | T          | F            |
         * | T        | F          | T            |
         */
        if ((*target_col)->data_type == INT) {
          bool data_eqal = (*target_col)->data.int_data[j] == atoi(value);
          get_index[j] = Xor(with_not, data_eqal);

        }else if ((*target_col)->data_type == STRING) {
          bool data_eqal = strcmp((*target_col)->data.string_data[j], value);
          // we need !data_eqal here since the return of strcmp
          get_index[j] = Xor(with_not, !data_eqal);

        }else if ((*target_col)->data_type == FLOAT) {
          bool data_eqal = (*target_col)->data.float_data[j] == strtof(value,0);
          get_index[j] = Xor(with_not, data_eqal);
        }
      }
    }
  }else if (lo == OR) {
    for (int j = 0; j < t->data_len; j++) {
      // false since OR
      if (get_index[j] == false){
        if ((*target_col)->data_type == INT) {
          bool data_eqal = (*target_col)->data.int_data[j] == atoi(value);
          get_index[j] = Xor(with_not, data_eqal);

        }else if ((*target_col)->data_type == STRING) {
          bool data_eqal = strcmp((*target_col)->data.string_data[j], value);
          // we need !data_eqal here since the return of strcmp
          get_index[j] = Xor(with_not, !data_eqal);

        }else if ((*target_col)->data_type == FLOAT) {
          bool data_eqal = (*target_col)->data.float_data[j] == strtof(value,0);
          get_index[j] = Xor(with_not, data_eqal);
        }
      }
    }
  }else if (lo == NONE){
    for (int j = 0; j < t->data_len; j++) {
      if ((*target_col)->data_type == INT) {
        bool data_eqal = (*target_col)->data.int_data[j] == atoi(value);
        get_index[j] = Xor(with_not, data_eqal);

      }else if ((*target_col)->data_type == STRING) {
        bool data_eqal = strcmp((*target_col)->data.string_data[j], value);
        // we need !data_eqal here since the return of strcmp
        get_index[j] = Xor(with_not, !data_eqal);

      }else if ((*target_col)->data_type == FLOAT) {
        bool data_eqal = (*target_col)->data.float_data[j] == strtof(value,0);
        get_index[j] = Xor(with_not, data_eqal);
      }
    }
  }
}
