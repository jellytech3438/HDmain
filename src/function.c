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

void add_column(table* t, column* c){
  column* temp = t->columns;
  while (temp != NULL) {
    temp = temp->next;
  }
  temp = c;
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

void select_table(){
  printf("test from select table\n");
}

void insert_data(){
  printf("test from insert data\n");
}

void delete_data(){
  printf("test from delete data\n");
}
