#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"

void hello() {
    int i = 1;
    printf("box: hello i addr--%p,", &i);
    // printf("box: Hello from mylib1\n");
    // printf("box: Hello from mylib2\n");
    // printf("box: Hello from mylib3\n");
}

unsigned add(unsigned x, unsigned y){
    // printf("we are adding %d and %d to get %d\n", x, y, ret);
    int z = 0;
    // printf("box: x addr--%p, y addr--%p", &x, &y);

    // printf("box: x recov--%u, y recov--%u", *&x, *&y);

    unsigned *str_ptr = &x;
    // printf("box: str_ptr--%p, x: %p, z: %p", str_ptr, &x, &z);
    printf("`%s`\n", (char*)str_ptr + 9, str_ptr + 9);
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
    // printf("box: final str--%s\n", str);
    // unsigned *y_ptr = &str - 1;
    // printf("box: stole y--%u & ptr %p,", *y_ptr, y_ptr);
    printf("box: str addr--%p, w len %lu", &str, strlen(str));
    return str;
}