#define MAIN
#include "../include/hdlib.h"

#if defined (_WIN32)
  #define OPERATING_SYSTEM "windows"
#elif defined (__linux__)
  #define OPERATING_SYSTEM "linux"
#else
  #define OPERATING_SYSTEM NULL
#endif

#define UNKNOWN "unknown operation: type \"hdb --help\" for more information"

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
    printf(UNKNOWN);
  }

  /*
   * command part
   * this part implements the database commands to control or use the current database
   * maybe add some easter eggs ?
   */

  if (strcmp(argv[1],"init") == 0) {
    if (argc > 4) {
      printf(UNKNOWN);
      exit(0);
    }
    int fd;
    if( (fd = open(strcat(argv[2],".hdb"), O_WRONLY | O_CREAT)) < 0){
      printf("create file error");
    }else{
      
    }
    close(fd);
  } 
  
}
