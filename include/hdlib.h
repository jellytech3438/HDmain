/*
 * here are the file that need to be include
 */

#ifdef DEBUG
#endif

#ifndef _HDLIB_
#define _HDLIB_
#define VERSION "beta0.0.1"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wchar.h>
#include <locale.h>
#include "function.h"
#include "datastruct.h"
#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))
#define MAXCHARSIZE 1024
// print color
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
// print box drawing char
const wchar_t
    VRT  = L'\x2551', // ║
    HRZ2 = L'\x2550', // ═
    HRZ1 = L'\x2500', // ─
    TUP  = L'\x2569', // ╩
    TDWN = L'\x2564', // ╤
    INTR = L'\x256B', // ╫
    LLCNR = L'\x255A', //╚
    LUCNR = L'\x2554', // ╔
    RLCNR = L'\x255D', // ╝
    RUCNR = L'\x2557', //╗
    LINTR = L'\x2560', // ╠
    RINTR = L'\x2563'; // ╣
#endif 
