#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"

int SECRET_NUM = 0xeeeeeeee;
static const int CONST_SECRET_NUM = 0xdddddddd;
const int CONST_SECRET_NUM2 = 0xcccccccc;
const int CONST_SECRET_NUM3 = 0xbbbbbbbb;
const int CONST_SECRET_NUM4 = 0xaaaaaaaa;

void hello() {
  printf("box: Hello from mylib1\n");
  printf("box: Hello from mylib2\n");
  printf("box: Hello from mylib3\n");
}

unsigned add(unsigned x, unsigned y){
    // printf("we are adding %d and %d to get %d\n", x, y, ret);
    return x + y;
}

int* leak_secret_num() {
    return &SECRET_NUM;
}

int* leak_const_secret_num() {
    // int *ptr = &CONST_SECRET_NUM;
    // *ptr = 102;
    // printf("const at %p", ptr);
    return &CONST_SECRET_NUM;
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
    return str;
}

void print_stdout() { 
    fprintf(stdout, "box: hello im in stdout\n");
}

void print_stderr(){
    fprintf(stderr, "box: hello im in stderr\n");
}

long* m() {
    long i = 0xdefaced;
    long j = 0xdeadbeef;
    long k = 0xfacedff;

    printf("i: %p, j: %p, k: %p\n", &i, &j, &k);
    // printf("gabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgogabagoogabooogaoaosgaogsodooaogoasgo\n");
    fprintf(stderr, "skskxskskskkskskskskkdkdskskkskakdkskklsklklsklskldklsklskldlskskxskskskkskskskskkdkdskskkskakdkskklsklklsklskldklsklskldlskskxskskskkskskskskkdkdskskkskakdkskklsklklsklskldklsklskldl\n");
    fprintf(stderr, "errors\n");
    fprintf(stderr, "no this cant be right wth\n");
    fprintf(stderr, "im printing lik ea gagillion times\n");

    // FIXME: wth can you print to stderr multiple times??

    return &k;
}

long* l() {
    long i = 0xcaffed;
    long j = 0xf4ffed;
    long k = 0;

    return m();
}

long* h() {
    long i = 0;
    long j = 0;
    long k = 0;

    return l();
}

long* f() {
    long i = 0;
    long j = 0;
    long k = 0;

    return h();
}

long* e() {
    long i = 0;
    long j = 0;
    long k = 0;

    return f();
}

long* d() {
    long i = 0;
    long j = 0;
    long k = 0;

    return e();
}

long* c() {
    long i = 0;
    long j = 0;
    long k = 0;

    return d();
}

long* b() {
    long i = 0;
    long j = 0;
    long k = 0;

    return c();
}

long* a() {
    long i = 0;
    long j = 0;
    long k = 0;

    return b();
}