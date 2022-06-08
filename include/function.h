#ifndef FNC_H
#define FNC_H
#include "../src/function.c"

/*
 * below are others functions
 */
extern void read_from_file(table* cur_table,FILE* opentable);
extern void save_data(table *newtable);
extern void split_column_type(char* args, char* column, char* type);
extern void split_column_value(char* args, char* column, char* value);
extern void check_hidden_folder();
extern bool file_exist(char *path);
extern bool dir_exist(char *path);
extern choosen check_yn(char *input);
extern bool Xor(bool a, bool b);

/*
 * below are datastruction functions
 */

extern void add_column(table* t, column* c);
extern void delet_column(table* t, char* c_name);
extern column** find_column_by_name(table* t, char* c_name);

/*
 * below are database functions
 */

extern void init_database(char* pathname);
extern void create_table(table* newtable);
extern void delete_data(table* cur_table, int index);
extern void plot_all_data(table* cur_table);
extern void plot_some_data(table* cur_table, bool* get_index);
extern void where_tag(table* t, bool* get_index,logicoperation lo, bool with_not, char* input);
#endif
