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

#define UNKNW_CMD RED"[Unknown command]"RESET" type \"hdb --help\" for more information"
#define CREAT_DUP_FILE "the file is already exist. are you sure to create anyway?[y/N]"
#define OPEN_TYPE_ERROR "can not open the file, please select the file with \".hdb\" extension"
#define OPEN_BROK_ERROR "the file is broken for some reason, please select the checkpoint file in the \"tmphdb\" folder"
#define OPEN_DUPL_ERROR "the file name is already exist, would you want create anyway?[Y/N]"
#define DELE_WARN "are you sure you want to delete anyway?[Y/N]"

int main(int argc, char **argv){

  setlocale(LC_ALL, "");

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  
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
    if (argc != 3) {
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

    if (mkdir(checkdir, 0755) < 0) {
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

    if (argc < 5) {
      printf(UNKNW_CMD);
      exit(0);
    }

    table* newtable = (table*)malloc(sizeof(table));
    newtable->column_len = 0;
    newtable->data_len = 0;
    char columname[MAXCHARSIZE] = {0};

    for (int i = 2; i < argc; i++) {
      if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "table") == 0) {
        i++;
        if (file_exist(argv[i])) {
          printf("table already exist\n");
          exit(0);
        }
        newtable->name = argv[i];
      }else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "column") == 0) {
        i++;
        int _t = i;
        while (i < argc) {
          char type[MAXCHARSIZE] = {0};
          column *cur_column = (column*)malloc(sizeof(column));
          split_column_value1(argv[i], cur_column->name, type, ':');

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

          add_column(newtable,cur_column);

          memset(columname,0,sizeof(columname));

          i++;
        }
        break;
      }
    }

    check_hidden_folder();
    write_to_file1(newtable);

  } else if (strcmp(argv[1],"insert") == 0) {
    /*
     * insert data to table
     * ex: hdb insert table_name values for_column1 for_column2 ...
     */
     check_hidden_folder();

     if (argc < 5) {
       printf(UNKNW_CMD);
       exit(0);
     }

     FILE *opentable;
     table *cur_table = (table*)malloc(sizeof(table));

     opentable = fopen (argv[2], "r");
     if (opentable == NULL) {
       printf("open table error\n");
       exit(0);
     }else{
       read_from_file1(cur_table,opentable);
     }

     if (strcmp(argv[3],"values") == 0) {
       if (cur_table->column_len != argc-4) {
         printf(RED"[Wrong length input]"RESET" Expected: %d, Receive: %d\n",cur_table->column_len, argc-4);
         exit(0);
       }
       column* temp_col = cur_table->columns;
       for (int i = 4; i < argc; i++) {
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
     }else{
       printf(UNKNW_CMD);
       exit(0);
     }

     write_to_file1(cur_table);
     plot_all_data(cur_table);

   } else if (strcmp(argv[1],"alter") == 0) {
    /*
     * delete inserted data or table
     * ex: hdb alter table_name add column:data_type
     * ex: hdb alter table_name drop column
     * ex: hdb alter table_name modify column data_type
     */
     check_hidden_folder();

     if (argc < 5) {
       printf(UNKNW_CMD);
       exit(0);
     }

     FILE *opentable;
     table *cur_table = (table*)malloc(sizeof(table));

     opentable = fopen(argv[2], "r");
     if (opentable == NULL) {
       printf("open table error\n");
       exit(0);
     }else{
       read_from_file1(cur_table,opentable);
     }

     if (strcmp(argv[3],"add") == 0) {
       char type[MAXCHARSIZE] = {0};
       column *newcol = (column*)malloc(sizeof(column));
       split_column_value1(argv[4], newcol->name, type, ':');
       if (strcmp(type, "int") == 0) {
         newcol->data_type = INT;
       }else if (strcmp(type, "string") == 0) {
         newcol->data_type = STRING;
       }else if (strcmp(type, "float") == 0) {
         newcol->data_type = FLOAT;
       }else{
         printf("unknown type\n");
         exit(0);
       }
       add_column(cur_table,newcol);
     }else if (strcmp(argv[3],"drop") == 0) {
       delet_column(cur_table,argv[4]);
     }else if (strcmp(argv[3],"modify") == 0) {
       if (argc != 6) {
         printf("INPUT error\n");
         exit(0);
       }
       column** target = find_column_by_name(cur_table, argv[4]);
       int origin_data_type = (*target)->data_type;
       if (strcmp(argv[5], "int") == 0) {
         if ((*target)->data_type == INT){
           printf("selected column type is what you want\n");
           exit(0);
         }
         (*target)->data_type = INT;
       }else if (strcmp(argv[5], "string") == 0) {
         if ((*target)->data_type == STRING){
           printf("selected column type is what you want\n");
           exit(0);
         }
         (*target)->data_type = STRING;
       }else if (strcmp(argv[5], "float") == 0) {
         if ((*target)->data_type == FLOAT){
           printf("selected column type is what you want\n");
           exit(0);
         }
         (*target)->data_type = FLOAT;
       }else{
         printf("unknown type\n");
         exit(0);
       }

       // change all exist data type

       if (origin_data_type == INT) {
         int origin_data[MAXCHARSIZE] = {0};
         for (int data_index = 0; data_index < cur_table->data_len; data_index++) {
           origin_data[data_index] = (*target)->data.int_data[data_index];
         }
         if ((*target)->data_type == STRING) {
           for (int data_index = 0; data_index < cur_table->data_len; data_index++) {
              char str[MAXCHARSIZE];
              sprintf(str, "%d", origin_data[data_index]);
              strcpy((*target)->data.string_data[data_index], str);
           }
         }else if ((*target)->data_type == FLOAT) {
           for (int data_index = 0; data_index < cur_table->data_len; data_index++) {
             (*target)->data.float_data[data_index] = origin_data[data_index];
           }
         }
       }else if (origin_data_type == STRING) {
         char origin_data[MAXCHARSIZE][MAXCHARSIZE] = {{0}};
         for (int data_index = 0; data_index < cur_table->data_len; data_index++) {
           strcpy(origin_data[data_index], (*target)->data.string_data[data_index]);
         }
         if ((*target)->data_type == INT) {
           for (int data_index = 0; data_index < cur_table->data_len; data_index++) {
             (*target)->data.int_data[data_index] = atoi(origin_data[data_index]);
           }
         }else if ((*target)->data_type == FLOAT) {
           for (int data_index = 0; data_index < cur_table->data_len; data_index++) {
             (*target)->data.float_data[data_index] = strtof(origin_data[data_index],0);
           }
         }
       }else if (origin_data_type == FLOAT) {
         float origin_data[MAXCHARSIZE] = {0.};
         for (int data_index = 0; data_index < cur_table->data_len; data_index++) {
           origin_data[data_index] = (*target)->data.float_data[data_index];
         }
         if ((*target)->data_type == INT) {
           for (int data_index = 0; data_index < cur_table->data_len; data_index++) {
             (*target)->data.int_data[data_index] = origin_data[data_index];
           }
         }else if ((*target)->data_type == STRING) {
           for (int data_index = 0; data_index < cur_table->data_len; data_index++) {
              // char temp = origin_data[data_index]+'0';
              char str[MAXCHARSIZE];
              sprintf(str, "%f", origin_data[data_index]);
              strcpy((*target)->data.string_data[data_index], str);
           }
         }
       }
     }else{
       printf(UNKNW_CMD);
       exit(0);
     }

     write_to_file1(cur_table);

   } else if (strcmp(argv[1],"update") == 0) {
     /*
     * update data in table
     * ex: hdb update table_name set column_name=new_data where condition
     */
     check_hidden_folder();
     if (argc < 5) {
       printf(UNKNW_CMD);
       exit(0);
     }
     FILE *opentable;
     table *cur_table = (table*)malloc(sizeof(table));

     opentable = fopen(argv[2], "r");
     if (opentable == NULL) {
       printf("open table error\n");
       exit(0);
     }else{
       read_from_file1(cur_table,opentable);
     }

     if (strcmp(argv[3],"set") == 0) {
       char colname[MAXCHARSIZE] = {0};
       char newdata[MAXCHARSIZE] = {0};
       split_column_value1(argv[4],colname,newdata,'=');

       bool get_index[cur_table->data_len];
       memset(get_index, false, cur_table->data_len * sizeof(bool));

       // dealing with index in where tag part
       if (strcmp(argv[5],"where") == 0) {
         int cur_arg = 6;
         while (cur_arg < argc) {
           logicoperation lo = NONE;
           bool with_not = false;
           if (strcmp(argv[cur_arg],"AND") == 0) {
             cur_arg++;
             lo = AND;
           }else if (strcmp(argv[cur_arg],"OR") == 0) {
             cur_arg++;
             lo = OR;
           }
           if (strcmp(argv[cur_arg],"NOT") == 0) {
             cur_arg++;
             with_not = true;
           }
           where_tag(cur_table,get_index,lo, with_not, argv[cur_arg]);
           cur_arg++;
         }
       }

       column** cur_column = find_column_by_name(cur_table,colname);
       for (int index = 0; index < cur_table->data_len; index++) {
         if (get_index[index] == true) {
           printf("%d\n", index);
           if ((*cur_column)->data_type == INT) {
             (*cur_column)->data.int_data[index] = atoi(newdata);
           }else if ((*cur_column)->data_type == STRING) {
             strcpy((*cur_column)->data.string_data[index], newdata);
           }else if ((*cur_column)->data_type == FLOAT) {
             (*cur_column)->data.float_data[index] = strtof(newdata,0);
           }
         }
       }

       write_to_file1(cur_table);
     }else{
       printf(UNKNW_CMD);
       exit(0);
     }

   } else if (strcmp(argv[1],"delete") == 0) {
    /*
     * delete inserted data or table
     * ex: hdb delete table_name where column_name=data
     * ex: hdb delete table_name where not column_name=data
     * ex: hdb delete table_name where column1_name=data and column2_name=data
     * ex: hdb delete table_name where column1_name=data or column2_name=data
     */
     check_hidden_folder();
     if (argc < 5) {
       printf(UNKNW_CMD);
       exit(0);
     }
     FILE *opentable;
     table *cur_table = (table*)malloc(sizeof(table));

     opentable = fopen(argv[2], "r");
     if (opentable == NULL) {
       printf("open table error\n");
       exit(0);
     }else{
       read_from_file1(cur_table,opentable);
     }

     bool get_index[cur_table->data_len];
     memset(get_index, false, cur_table->data_len * sizeof(bool));

     // dealing with index in where tag part
     if (strcmp(argv[3],"where") == 0) {
       int cur_arg = 4;
       while (cur_arg < argc) {
         logicoperation lo = NONE;
         bool with_not = false;
         if (strcmp(argv[cur_arg],"AND") == 0) {
           cur_arg++;
           lo = AND;
         }else if (strcmp(argv[cur_arg],"OR") == 0) {
           cur_arg++;
           lo = OR;
         }
         if (strcmp(argv[cur_arg],"NOT") == 0) {
           cur_arg++;
           with_not = true;
         }
         where_tag(cur_table,get_index,lo, with_not, argv[cur_arg]);
         cur_arg++;
       }

       for (int index = 0; index < cur_table->data_len; index++) {
         if (get_index[index] == true) {
           delete_data(cur_table,index);
         }
       }
       write_to_file1(cur_table);
     }else{
       printf(UNKNW_CMD);
       exit(0);
     }

  } else if (strcmp(argv[1],"search") == 0) {
    /*
     * search for specific data
     * ex: hdb search table_name where condition
     */
     check_hidden_folder();
     if (argc < 3) {
       printf(UNKNW_CMD);
       exit(0);
     }
     FILE *opentable;
     table *cur_table = (table*)malloc(sizeof(table));

     opentable = fopen(argv[2], "r");
     if (opentable == NULL) {
       printf("open table error\n");
       exit(0);
     }else{
       read_from_file1(cur_table,opentable);
     }

     bool get_index[cur_table->data_len];
     memset(get_index, false, cur_table->data_len * sizeof(bool));

     // dealing with index in where tag part
     if (strcmp(argv[3],"where") == 0) {
       int cur_arg = 4;
       while (cur_arg < argc) {
         logicoperation lo = NONE;
         bool with_not = false;
         if (strcmp(argv[cur_arg],"AND") == 0) {
           cur_arg++;
           lo = AND;
         }else if (strcmp(argv[cur_arg],"OR") == 0) {
           cur_arg++;
           lo = OR;
         }
         if (strcmp(argv[cur_arg],"NOT") == 0) {
           cur_arg++;
           with_not = true;
         }
         where_tag(cur_table,get_index,lo, with_not, argv[cur_arg]);
         cur_arg++;
       }
     }else if (strcmp(argv[3],"ALL") == 0){
       memset(get_index, true, cur_table->data_len * sizeof(bool));
     }else{
       printf(UNKNW_CMD);
       exit(0);
     }

     plot_some_data(cur_table, get_index);

  } else if (strcmp(argv[1],"rename") == 0) {
    /*
     * rename a table or column
     * ex: hdb rename -t origin_name new_name
     * ex: hdb rename -t table_name -c origin_name new_name
     * ==> -c means columns -t means table
     */
     check_hidden_folder();
     if (argc < 5) {
       printf(UNKNW_CMD);
       exit(0);
     }
     FILE *opentable;
     table *cur_table = (table*)malloc(sizeof(table));

     opentable = fopen(argv[3], "r");
     if (opentable == NULL) {
       printf("open table error\n");
       exit(0);
     }else{
       read_from_file1(cur_table,opentable);
     }

     if (strcmp(argv[4], "-c") == 0 || strcmp(argv[4], "column") == 0) {
       if (argc < 7) {
         printf(UNKNW_CMD);
         exit(0);
       }
       column** target_col = find_column_by_name(cur_table, argv[5]);
       strcpy((*target_col)->name,argv[6]);
       write_to_file1(cur_table);
     }else {
        int result = rename(argv[3], argv[4]);
        if (result == 0) {
          printf("The table is renamed successfully.");
        } else {
          printf("The table renamed is fail.");
          exit(0);
        }
     }

  } else if (strcmp(argv[1],"plot") == 0) {
    /*
     * rename a table or column
     * ex: hdb plot [--bar / --line] table_name -x column -y column
     * ==> y can only be INT or FLOAT type
     */
  } else {
    printf(UNKNW_CMD);
  }
  
  lua_close(L);

}
