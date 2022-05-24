/*
 * these are types that need to be implemented
 * 1. building
 * 2. floor
 * 3. room
 * 4. type of tenant
 * please make sure all of the type of data used correctly
 */

#ifndef DS_H
#define DS_H

/*
 * below are datastructures for database
 */

typedef struct room{
  void* data;
  struct room *next;
} room;

typedef struct floor{
  room *rooms;
  struct floor *next;
} floor;

typedef struct table{
  char *name;
  floor *floors;
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

// all extern variables are below

extern choosen choose;

#endif
