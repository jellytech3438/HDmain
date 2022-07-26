#include <stdio.h>
#include <stdlib.h>
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
#include "printfunction.c"
#include "iofunction.c"

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
