#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "../include/hdlib.h"

#define DEBUG

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

  setlocale(LC_ALL, "");

  choosen choose;

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
     * ==> create a dir called mydb which contains .hdb folder as hidden to operate with database
     */
    if (argc > 4) {
      printf(UNKNW_CMD);
      exit(0);
    }

    char* dup_dir_tag = "_copy";
    char* checkdir = argv[2];
    while (dir_exist(checkdir)) {
      char ip[MAXCHARSIZE];
      printf(CREAT_DUP_FILE);
      fgets(ip,sizeof(ip),stdin);
      while((choose = check_yn(ip)) == ELSE){
        printf(CREAT_DUP_FILE);
        fgets(ip,sizeof(ip),stdin);
      }
      if (choose == NO || choose == DEFAULT){
        printf("create directory failed\n");
        exit(0);
      }else{
        checkdir = strcat(checkdir,dup_dir_tag);
#ifdef DEBUG
      printf("%s\n",argv[2]);
#endif
      }
    }

    // still choosing using file or directory
    // current: dir

    if (mkdir(checkdir, 0777) < 0) {
      printf("Create Directory error");
    }else{
      init_database(checkdir);
    }

  } else if (strcmp(argv[1],"create") == 0) {
    /*
     * create table and initialize all data to zero
     * ex: hdb create table table_name -c column1:int column2:string
     * ==> create a table with following columns whose type are beside :
     */

    if (opendir(".hdb") < 0) {
      printf("error database");
      exit(0);
    }

    table* newtable = (table*)malloc(sizeof(table));
    newtable->column_len = 0;
    newtable->data_len = 0;
    char columname[MAXCHARSIZE] = {0};

    for (int i = 2; i < argc; i++) {
      if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "table") == 0) {
        i++;
        newtable->name = argv[i];
      }else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "column") == 0) {
        i++;
        int _t = i;
        while (i < argc) {
          char* split;
          if ((split = strchr(argv[i], ':')) == NULL) {
            newtable->column_rule = NO_RULE;
          }else{
            newtable->column_rule = COMPLETE;

            column *cur_column = (column*)malloc(sizeof(column));

            char type[MAXCHARSIZE] = {0};

            int j = 0;

            for (; argv[i][j] != *split; j++) {
              cur_column->name[j] = argv[i][j];
            }

            j++;

            for (int k = 0;argv[i][j] != 0; j++,k++) {
              type[k] = argv[i][j];
            }

#ifdef DEBUG
            printf("%s %s\n",cur_column->name,type);
#endif

            if (strcmp(type, "int") == 0) {
              cur_column->data_type = INT;
            }else if (strcmp(type, "string") == 0) {
              cur_column->data_type = STRING;
            }else if (strcmp(type, "float") == 0) {
              cur_column->data_type = FLOAT;
            }else{
              printf("unknown type\n");
              exit(0);
            }

            if (_t == i) {
              newtable->columns = cur_column;
            }else{
              column **temp = &newtable->columns;
              while(*temp != NULL)
                temp = &(*temp)->next;
              *temp = cur_column;
            }

            newtable->column_len++;

            memset(columname,0,sizeof(columname));
          }
          i++;
        }
        break;
      }
    }

    create_table(newtable);

  } else if (strcmp(argv[1],"insert") == 0) {
    /*
     * insert data to table
     * ex: hdb insert into table_name values for_column1 for_column2 ...
     */
     check_hidden_folder();

     FILE *opentable;
     table *cur_table = (table*)malloc(sizeof(table));

     for (int i = 2; i < argc; i++) {
       if (strcmp(argv[i],"into") == 0) {
         i++;
         opentable = fopen (argv[i], "r");
         if (opentable == NULL) {
           printf("open table error\n");
           exit(0);
         }else{

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

           while(fread(temp,sizeof(column),1,opentable)){
              strcpy(last->name, temp->name);
              last->next = (column*)malloc(sizeof(column));
              last = last->next;
           }

         }
         printf("%s\n%s\n", cur_table->columns->name, cur_table->columns->next->name);
         exit(0);
       }else if (strcmp(argv[i],"values") == 0) {
         if (cur_table->column_len != argc-i-1) {
           printf("error input (wrong len)\n");
           exit(0);
         }else{
           i++;
           column* temp_col = cur_table->columns;
           for (int _ = 0; _ < cur_table->column_len; _++) {
             if (temp_col->data_type == INT) {
               temp_col->data.int_data[cur_table->data_len] = atoi(argv[i]);
             }else if (temp_col->data_type == STRING) {
               strcpy(temp_col->data.string_data[cur_table->data_len], argv[i]);
             }else if (temp_col->data_type == FLOAT) {
               temp_col->data.float_data[cur_table->data_len] = strtof(argv[i],0);
             }
             temp_col = temp_col->next;
           }
           cur_table->data_len++;
         }
         printf("%d\n", cur_table->columns->data.int_data[0]);
       }
     }

     save_data(cur_table);

   } else if (strcmp(argv[1],"alter") == 0) {
    /*
     * delete inserted data or table
     * ex: hdb alter table_name add column
     * ex: hdb alter table_name drop column
     * ex: hdb alter table_name modify column data_type
     */
     check_hidden_folder();
   } else if (strcmp(argv[1],"update") == 0) {
    /*
     * update data in table
     * ex: hdb update table_name set column_name=new_data where condition
     */
     check_hidden_folder();
   } else if (strcmp(argv[1],"delete") == 0) {
    /*
     * delete inserted data or table
     * ex: hdb delete from table_name where condition
     */
  } else if (strcmp(argv[1],"search") == 0) {
    /*
     * search for specific data
     * ex: hdb search table_name where condition
     */
  } else if (strcmp(argv[1],"rename") == 0) {
    /*
     * rename a table or column
     * ex: hdb rename -t origin_name new_name
     * ex: hdb rename -c origin_name new_name
     * ==> -c means columns -t means table
     */
     check_hidden_folder();

  } else {
    printf(UNKNW_CMD);
  }

}
