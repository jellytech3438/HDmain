#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../include/datastruct.h"

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

