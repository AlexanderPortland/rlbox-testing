#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    void hello();
    unsigned add(unsigned, unsigned);
    char *naive_concat(char *, char *);
    int* leak_secret_num();
    int* leak_const_secret_num();
    long* a();
    void print_stdout();
    void print_stderr();
    // void echo(const char* str);
    // void call_cb(void (*cb) (const char* str));
#ifdef __cplusplus
}
#endif