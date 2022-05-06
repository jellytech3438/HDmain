#include <stdio.h>
#include <string.h>
#define MAIN
#include "../include/hdlib.h"

#if defined (_WIN32)
  #define OPERATING_SYSTEM "windows"
#elif defined (__linux__)
  #define OPERATING_SYSTEM "linux"
#else
  #define OPERATING_SYSTEM NULL
#endif

#define UNKNW_CMD "unknown command: type \"hdb --help\" for more information"
#define OPEN_TYPE_ERROR "can not open the file, please select the file with \".hdb\" extension"
#define OPEN_BROK_ERROR "the file is broken for some reason, please select the checkpoint file in the \"\""
#define OPEN_DUPL_ERROR "the file name is already exist, would you want create anyway?[Y/N]"
#define DELE_WARN "are you sure you want to delete anyway?[Y/N]"

int main(int argc, char **argv){
  
  /*
   * prefix(tags) part
   * the command line support users to have flag to do the specific operation
   * attempt to add manual file to implement help commmand
   */

  if (strcmp(argv[1],"--version") == 0 || strcmp(argv[1], "-v") == 0) printf("hdb " VERSION);exit(0);
    
  if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0){
    system("man hdb");
  }else{
    printf(UNKNW_CMD);
  }

  /*
   * command part
   * this part implements the database commands to control or use the current database
   * maybe add some easter eggs ?
   */

  if (strcmp(argv[1],"init") == 0) {
    if (argc > 4) {
      printf(UNKNW_CMD);
      exit(0);
    }
    int fd;
    if( (fd = open(strcat(argv[2],".hdb"), O_WRONLY | O_CREAT)) < 0){
      printf("create file error");
    }else{
      
    }
    close(fd);
  } else if (strcmp(argv[1],"select") == 0) {

  } else if (strcmp(argv[1],"create") == 0) {

  } else if (strcmp(argv[1],"insert") == 0) {
  
  } else if (strcmp(argv[1],"delete") == 0) {
  
  } else if (strcmp(argv[1],"search") == 0) {

  } else {
    printf(UNKNW_CMD);
  }
  
}
