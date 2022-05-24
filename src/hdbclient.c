#include <stddef.h>
#include <stdio.h>
#include <dirent.h>

#define MAIN
#include "../include/hdlib.h"

struct opendb{
  int fd;
  char *name;
};

int main (int argc, char *argv[]){
  
  if (opendir(".hdb") < 0) {
    printf("open error");
    exit(0);
  }

  // prefix operation here

  // print logo here

  char *buffer;
  size_t bufsize = MAXCHARSIZE;
  struct opendb curdb = {-1,NULL};

  while (1) {
    printf("%s >>", curdb.fd == -1 ? "hdb" : curdb.name);
    char cmd = getline(&buffer,&bufsize,stdin);
    
  }

  return 0;
}
