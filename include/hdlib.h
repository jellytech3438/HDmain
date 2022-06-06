/*
 * here are the file that need to be include
 * please make safty check to prevent error
 */

#ifdef DEBUG
#endif

#ifdef MAIN
#define VERSION "beta0.0.1"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "function.h"
#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))
#define MAXCHARSIZE 1024
#endif 
