#ifndef FNC_H
#define FNC_H
#include "../src/function.c"

/*
 * below are file io functions
 */
extern void read_from_file(table* cur_table,FILE* opentable);
extern void write_to_file1(table *newtable);

/*
 * below are some basic check and operation functions
 */
extern void split_column_value1(char* args, char* column, char* value, char split);
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
extern void delete_data(table* cur_table, int index);
extern void plot_all_data(table* cur_table);
extern void plot_some_data(table* cur_table, bool* get_index);
extern void where_tag(table* t, bool* get_index,logicoperation lo, bool with_not, char* input);
#endif
