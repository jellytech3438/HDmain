#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../include/datastruct.h"

/*
 * below are the datastruction function
 */

/*
 * below are the command line function
 */

void init_database(){
  printf("test from init database\n");
}

void create_table(){
  printf("test from create table\n");
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

/*
 * below are others functions
 */

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
