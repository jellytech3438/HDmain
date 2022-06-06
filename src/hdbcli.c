#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
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
#define CREAT_DUP_FILE "the file is already exist. are you sure to create anyway?[y/N]"
#define OPEN_TYPE_ERROR "can not open the file, please select the file with \".hdb\" extension"
#define OPEN_BROK_ERROR "the file is broken for some reason, please select the checkpoint file in the \"tmphdb\" folder"
#define OPEN_DUPL_ERROR "the file name is already exist, would you want create anyway?[Y/N]"
#define DELE_WARN "are you sure you want to delete anyway?[Y/N]"

int main(int argc, char **argv){
  
  /*
   * prefix(tags) part
   * the command line support users to have flag to do the specific operation
   * attempt to add manual file to implement help commmand
   */

  if (strcmp(argv[1],"--version") == 0 || strcmp(argv[1], "-v") == 0){
    printf("hdb " VERSION);
    exit(0);
  }else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0){
    system("man hdb");
  }

  /*
   * command part
   * this part implements the database commands to control or use the current database
   * maybe add some easter eggs ?
   */

  else if (strcmp(argv[1],"init") == 0) {
    /*
     * initialize a new database
     * ex: hdb init mydb
     * ==> create a file called mydb.hdb
     */
    if (argc > 4) {
      printf(UNKNW_CMD);
      exit(0);
    }
    if (file_exist(argv[2])) {
      choosen choos;
      printf(CREAT_DUP_FILE);
      char choose[MAXCHARSIZE];
      fgets(choose,sizeof(choose),stdin);
      while((choos = check_yn(choose)) == ELSE){
        printf(CREAT_DUP_FILE);
        fgets(choose,sizeof(choose),stdin);
      }
      if (choos == NO || choos == DEFAULT){
        printf("create file failed\n");
        exit(0);
      }
    }
    int fd;
    if( (fd = open(strcat(argv[2],".hdb"), O_WRONLY | O_CREAT)) < 0){
      printf("create file error");
    }else{
      init_database(fd);
    }
    close(fd);
  } else if (strcmp(argv[1],"select") == 0) {
    /*
     * select table for further operation
     * ex: hdb select table1
     * ==> find table1 and make pointer point to the offset
     */
  } else if (strcmp(argv[1],"create") == 0) {
    /*
     * create table and initialize all data to zero
     * ex: hdb create table1:r:num:c:num 
     * ==> create a table 
     */
  } else if (strcmp(argv[1],"insert") == 0) {
    /*
     * insert data to table
     * ex: hdb insert table1 name class {index}*/
  } else if (strcmp(argv[1],"delete") == 0) {
    /*
     * delete inserted data
     * ex: hdb delete name:all
     * ==> delete all file related to name
     * ==> trying to add some mode
     */
  } else if (strcmp(argv[1],"search") == 0) {
    /*
     * search for specific data
     * ex: hdb search Jack:class
     * ==> search Jake's class for example
     */
  } else {
    printf(UNKNW_CMD);
  }
  
}
