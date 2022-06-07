#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../include/datastruct.h"

/*
 * below are others functions
 */

void read_from_file(table* cur_table, FILE* opentable){
  if (!fread(cur_table,sizeof(table),1,opentable)) {
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

void save_data(table* newtable){
 FILE* table = fopen(newtable->name, "w");
 size_t length;

 if (fwrite(newtable, sizeof(struct table), 1, table) < 0){
   printf("%d\n", errno);
 }

 length = strlen(newtable->name) + 1;
 fwrite(&length, sizeof(length), 1, table);
 fwrite(newtable->name, 1, length, table);

 column *col = newtable->columns;
 while (col != NULL) {
   if(fwrite(col, sizeof(column), 1, table) < 0){
     printf("%d\n", errno);
   }
   col = col->next;
 }
}

void split_column_type(char* args, char* column, char* type){
  char* split;
  if ((split = strchr(args, ':')) == NULL) {
    printf("unacceptable no type column\n");
    exit(0);
  }else{
    int j = 0;

    for (; args[j] != *split; j++) {
      column[j] = args[j];
    }

    j++;

    for (int k = 0;args[j] != 0; j++,k++) {
      type[k] = args[j];
    }
  }
}

void split_column_value(char* args, char* column, char* value){
  char* split;
  if ((split = strchr(args, '=')) == NULL) {
    printf("unacceptable no type column\n");
    exit(0);
  }else{
    int j = 0;

    for (; args[j] != *split; j++) {
      column[j] = args[j];
    }

    j++;

    for (int k = 0;args[j] != 0; j++,k++) {
      value[k] = args[j];
    }
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
  return (stat(strcat(path,".hdb"), &buffer) == 0);
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

/*
 * below are the datastruction function
 */

column* new_column(char* input, char type[]){
  column *cur_column = (column*)malloc(sizeof(column));

  split_column_type(input, cur_column->name, type);

  return cur_column;
}

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
    return;
  }else{
    column **pre = &t->columns;
    column **cur = &t->columns;
    int i = 0;
    while(*cur != NULL){
      if (strcmp((*cur)->name, c_name) == 0) {
        // printf("%d %d\n",i , t->column_len-1);
        if (i == t->column_len-1) {
          (*pre)->next = NULL;
          return;
        }else{
          (*pre)->next = (*cur)->next;
          return;
        }
      }
      if (i != 0) {
        pre = &(*pre)->next;
      }
      cur = &(*cur)->next;
      i++;
    }
    t->column_len--;
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

void create_table(table* newtable){
  if(chdir(".hdb") < 0){
    printf("cd error");
  }

  printf("%s\n", newtable->name);

  int table_file;
  if ((table_file = open(newtable->name, O_WRONLY | O_CREAT | O_APPEND)) < 0) {
    printf("%d create error %d",table_file,errno);
    return;
  }

  chmod(newtable->name, 0755);

  save_data(newtable);
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

void plot_all_data(table* cur_table){
  printf("%s\n", cur_table->name);
  column **temp = &cur_table->columns;
  while(*temp != NULL){
    printf("\t|-%s\n", (*temp)->name);
    for (int i = 0; i < cur_table->data_len; i++) {
      if ((*temp)->data_type == INT) {
        printf("\t|\t|-%d\n", (*temp)->data.int_data[i]);
      }else if ((*temp)->data_type == STRING) {
        printf("\t|\t|-%s\n", (*temp)->data.string_data[i]);
      }else if ((*temp)->data_type == FLOAT) {
        printf("\t|\t|-%f\n", (*temp)->data.float_data[i]);
      }
    }
    temp = &(*temp)->next;
  }
}

void where_tag(table* t, bool* get_index,logicoperation lo, bool with_not, char* input) {
  // dealing with spliting input
  column **target_col;
  char* split;
  char column[MAXCHARSIZE] = {0};
  char value[MAXCHARSIZE] = {0};
  if ((split = strchr(input, '=')) == NULL) {
    printf("unacceptable no type column\n");
    exit(0);
  }else{
    split_column_value(input,column,value);
  }

  // index as condition
  if (strcmp(column,"INDEX") == 0) {
    int index = atoi(value);
    if (index > 0 && index < t->data_len) {
      get_index[index] = true;
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
        if ((*target_col)->data_type == INT) {
          if (with_not == false) {
            if ((*target_col)->data.int_data[j] == atoi(value)) {
              get_index[j] = true;
            }else{
              get_index[j] = false;
            }
          }else{
            if ((*target_col)->data.int_data[j] != atoi(value)) {
              get_index[j] = true;
            }else{
              get_index[j] = false;
            }
          }
        }else if ((*target_col)->data_type == STRING) {
          if (with_not == false) {
            if (strcmp((*target_col)->data.string_data[j], value) == 0) {
              get_index[j] = true;
            }else{
              get_index[j] = false;
            }
          }else{
            if (strcmp((*target_col)->data.string_data[j], value) != 0) {
              get_index[j] = true;
            }else{
              get_index[j] = false;
            }
          }
        }else if ((*target_col)->data_type == FLOAT) {
          if (with_not == false) {
            if ((*target_col)->data.float_data[j] == strtof(value,0)) {
              get_index[j] = true;
            }else{
              get_index[j] = false;
            }
          }else{
            if ((*target_col)->data.float_data[j] != strtof(value,0)) {
              get_index[j] = true;
            }else{
              get_index[j] = false;
            }
          }
        }
      }
    }
  }else if (lo == OR) {
    for (int j = 0; j < t->data_len; j++) {
      // false since OR
      if (get_index[j] == false){
        if ((*target_col)->data_type == INT) {
          if (with_not == false) {
            if ((*target_col)->data.int_data[j] == atoi(value)) {
              get_index[j] = true;
            }else{
              get_index[j] = false;
            }
          }else{
            if ((*target_col)->data.int_data[j] != atoi(value)) {
              get_index[j] = true;
            }else{
              get_index[j] = false;
            }
          }
        }else if ((*target_col)->data_type == STRING) {
          if (with_not == false) {
            if (strcmp((*target_col)->data.string_data[j], value) == 0) {
              get_index[j] = true;
            }else{
              get_index[j] = false;
            }
          }else{
            if (strcmp((*target_col)->data.string_data[j], value) != 0) {
              get_index[j] = true;
            }else{
              get_index[j] = false;
            }
          }
        }else if ((*target_col)->data_type == FLOAT) {
          if (with_not == false) {
            if ((*target_col)->data.float_data[j] == strtof(value,0)) {
              get_index[j] = true;
            }else{
              get_index[j] = false;
            }
          }else{
            if ((*target_col)->data.float_data[j] != strtof(value,0)) {
              get_index[j] = true;
            }else{
              get_index[j] = false;
            }
          }
        }
      }
    }
  }else if (lo == NONE){
    for (int j = 0; j < t->data_len; j++) {
      if ((*target_col)->data_type == INT) {
        if (with_not == false) {
          if ((*target_col)->data.int_data[j] == atoi(value)) {
            get_index[j] = true;
          }else{
            get_index[j] = false;
          }
        }else{
          if ((*target_col)->data.int_data[j] != atoi(value)) {
            get_index[j] = true;
          }else{
            get_index[j] = false;
          }
        }
      }else if ((*target_col)->data_type == STRING) {
        if (with_not == false) {
          if (strcmp((*target_col)->data.string_data[j], value) == 0) {
            get_index[j] = true;
          }else{
            get_index[j] = false;
          }
        }else{
          if (strcmp((*target_col)->data.string_data[j], value) != 0) {
            get_index[j] = true;
          }else{
            get_index[j] = false;
          }
        }
      }else if ((*target_col)->data_type == FLOAT) {
        if (with_not == false) {
          if ((*target_col)->data.float_data[j] == strtof(value,0)) {
            get_index[j] = true;
          }else{
            get_index[j] = false;
          }
        }else{
          if ((*target_col)->data.float_data[j] != strtof(value,0)) {
            get_index[j] = true;
          }else{
            get_index[j] = false;
          }
        }
      }
    }
  }
}
