// We're going to use RLBox in a single-threaded environment.
#define RLBOX_SINGLE_THREADED_INVOCATIONS

// All calls into the sandbox are resolved statically.
#define RLBOX_USE_STATIC_CALLS() rlbox_wasm2c_sandbox_lookup_symbol

// Provide the wasm2c module a name
#define RLBOX_WASM2C_MODULE_NAME mylib

#include <stdio.h>
#include <cassert>
#include "mylib.h"
#include "mylib.wasm.h"
#include "rlbox.hpp"
#include "rlbox_wasm2c_sandbox.hpp"
#include <stdlib.h>

using namespace std;
using namespace rlbox;

RLBOX_DEFINE_BASE_TYPES_FOR(mylib, wasm2c);

#define REMAKING_OUT_COLOR  "\x1b[93m" // 91 for red, 92 for green
#define RESETTING_OUT_COLOR "\x1b[94m"
#define COLOR_RESET         "\x1b[0m"

void reset_or_remake(rlbox_sandbox_mylib* sandbox, bool reset){
    if (reset) {
        printf("\tmain: " RESETTING_OUT_COLOR "resetting" COLOR_RESET " sandbox\n");
        sandbox->reset_sandbox();
    } else {
        printf("\tmain: " REMAKING_OUT_COLOR "remaking" COLOR_RESET " sandbox\n");
        sandbox->destroy_sandbox();
        sandbox->create_sandbox();
    }
}

void add_demo(rlbox_sandbox_mylib* sandbox, bool reset){
    auto C = std::rand() % 100;
    auto D = std::rand() % 1000;
    auto ok_num2 = sandbox->invoke_sandbox_function(add, C, D)
              .copy_and_verify([C, D](unsigned ret)
                               {
    printf("main: we are adding %d and %d to get %d\n", C, D, ret);
    return ret == (C + D); });

    reset_or_remake(sandbox, reset);
}

void str_demo(rlbox_sandbox_mylib* sandbox, bool reset){
    // define our strings
    const char* s1 = "hello this is ";
    size_t str1_sz = strlen(s1);
    const char *s2 = "a test string";
    size_t str2_sz = strlen(s2);

    // malloc them in the sandbox
    tainted_mylib<char*> taintedStr1 = sandbox->malloc_in_sandbox<char>(str1_sz);
    strncpy(taintedStr1.unverified_safe_pointer_because(str1_sz, "writing to region")
            , s1, str1_sz);
    tainted_mylib<char*> taintedStr2 = sandbox->malloc_in_sandbox<char>(str2_sz);
    strncpy(taintedStr2.unverified_safe_pointer_because(str2_sz, "writing to region")
            , s2, str2_sz);

    auto ok_str = sandbox->invoke_sandbox_function(naive_concat, taintedStr1, taintedStr2);

    char *str1 = taintedStr1.UNSAFE_unverified();
    char *str2 = taintedStr2.UNSAFE_unverified();
    char *str_final = ok_str.UNSAFE_unverified();

    printf("main: str1--%s\n", str1);
    printf("main: str2--%s\n", str2);
    printf("main: str_final--%s\n", str_final);

    reset_or_remake(sandbox, reset);

    printf("main: str1--%s\n", str1);
    printf("main: str2--%s\n", str2);
    printf("main: str_final--%s\n", str_final);
}

void secret_num_demo(rlbox_sandbox_mylib* sandbox, bool reset){
    auto sec_num_res = sandbox->invoke_sandbox_function(leak_secret_num);
    auto const_sec_num_res = sandbox->invoke_sandbox_function(leak_const_secret_num);

    int *secret_num_ptr = sec_num_res.UNSAFE_unverified();
    int *const_secret_num_ptr = const_sec_num_res.UNSAFE_unverified();

    printf("main: secret_num--0x%x\n", *secret_num_ptr);
    printf("main: const_secret_num--0x%x\n", *const_secret_num_ptr);

    reset_or_remake(sandbox, reset);

    printf("main: secret_num--0x%x\n", *secret_num_ptr);
    printf("main: const_secret_num--0x%x\n", *const_secret_num_ptr);
}