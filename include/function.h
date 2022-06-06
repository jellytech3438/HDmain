#ifndef FNC_H
#define FNC_H
#include "../src/function.c"

/*
 * below are others functions
 */
extern void read_from_file(table* cur_table,FILE* opentable);
extern void save_data(table *newtable);
extern void split_column_type(char* args, char* column, char* type);
extern void check_hidden_folder();
extern bool file_exist(char *path);
extern bool dir_exist(char *path);
extern choosen check_yn(char *input);

/*
 * below are datastruction functions
 */

extern column* new_column(char* input, char typr[]);
extern void add_column(table* t, column* c);
extern void delet_column(table* t, char* c_name);
extern column** find_column_by_name(table* t, char* c_name);

/*
 * below are database functions
 */

extern void init_database(char* pathname);
extern void create_table(table* newtable);

#endif
