#include <stdio.h>
#if defined (_WIN32)
  #define OPERATING_SYSTEM "windows"
#elif defined (__linux__)
  #define OPERATING_SYSTEM "linux"
#else
  #define OPERATING_SYSTEM NULL
#endif

#define MAIN 1
#include "../include/lib.h"

int main(int argc, char **argv){
  while(1){
    printf("test");    
  }
}