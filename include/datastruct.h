#ifndef DS_H
#define DS_H
#define MAXCHARSIZE 1024
/*
 * below are datastructures for database
 */

typedef struct linkedlist{
  char name[MAXCHARSIZE];
  enum {
    INT,
    STRING,
    FLOAT,
  } data_type;
  union {
    int int_data[MAXCHARSIZE];
    char string_data[MAXCHARSIZE][MAXCHARSIZE];
    float float_data[MAXCHARSIZE];
  } data;
  struct linkedlist *next;
} column;

typedef struct table{
  int column_len;
  int data_len;
  char *name;
  column *columns;
} table;

/*
 * below are others
 */

typedef enum choosen{
  DEFAULT,
  YES,
  NO,
  ELSE
} choosen;

typedef enum logicoperation{
  NONE,
  AND,
  OR,
} logicoperation;

typedef enum filtervalue{
  LT,
  EQ,
  BT
} filtervalue;

#endif
