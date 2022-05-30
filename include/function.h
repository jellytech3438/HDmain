#ifndef FNC_H
#define FNC_H
#include "../src/function.c"

/*
 * below are others functions
 */

extern void save_data(table *newtable);
extern void check_hidden_folder();
extern bool file_exist(char *path);
extern bool dir_exist(char *path);
extern choosen check_yn(char *input);

/*
 * below are datastruction functions
 */

extern void add_column(table* t, column* c);

/*
 * below are database functions
 */

extern void init_database(char* pathname);
extern void create_table(table* newtable);

#endif
