#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"

void hello() {
  printf("box: Hello from mylib1\n");
  printf("box: Hello from mylib2\n");
  printf("box: Hello from mylib3\n");
}

unsigned add(unsigned x, unsigned y){
    // printf("we are adding %d and %d to get %d\n", x, y, ret);
    return x + y;
}

char* naive_concat(char* x, char* y){
    // printf("Hello from mylib3\n");
    size_t total_len = strlen(x) + strlen(y);
    char *str = malloc(total_len);
    
    memcpy(str, x, strlen(x));
    
    memcpy(str + strlen(x), y, strlen(y));
    // printf("Hello from mylib4\n");
    // printf("Hello from mylib4 %s\n", y);
    printf("box: final str--%s\n", str);
    return str;
}