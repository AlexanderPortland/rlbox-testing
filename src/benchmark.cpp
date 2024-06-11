// We're going to use RLBox in a single-threaded environment.
#define RLBOX_SINGLE_THREADED_INVOCATIONS

// All calls into the sandbox are resolved statically.
#define RLBOX_USE_STATIC_CALLS() rlbox_wasm2c_sandbox_lookup_symbol

// Provide the wasm2c module a name
#define RLBOX_WASM2C_MODULE_NAME mylib

#include <stdio.h>
#include <cassert>
#include <chrono>
#include "mylib.h"
#include "mylib.wasm.h"
#include "rlbox.hpp"
#include "rlbox_wasm2c_sandbox.hpp"
#include <time.h>
#include <stdlib.h>

using namespace std;
using namespace std::chrono;
using namespace rlbox;

// Define base type for mylib using the noop sandbox
RLBOX_DEFINE_BASE_TYPES_FOR(mylib, wasm2c);

#define REMAKING_OUT_COLOR  "\x1b[93m" // 91 for red, 92 for green
#define RESETTING_OUT_COLOR "\x1b[94m"
#define COLOR_RESET         "\x1b[0m"

void str_test(rlbox_sandbox_mylib* sandbox){
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
}

void num_test(rlbox_sandbox_mylib* sandbox){
    auto C = rand();
    auto D = rand();
    auto ok_num2 = sandbox->invoke_sandbox_function(add, C, D)
              .copy_and_verify([C, D](unsigned ret)
                               {
    // printf("main: we are adding %d and %d to get %d\n", C, D, ret);
    return ret == (C + D); });
}

void call_function(rlbox_sandbox_mylib* sandbox) { 
    // TODO: make use function ptrs instead
    str_test(sandbox);
}

// preliminary results (w/ string function):
//    new every time -- 14,120,119 microseconds (for 1 mil string functions)
//                      13,436,902 microseconds
//                      14,053,163 microseconds
//                      13,538,190 microseconds
//                      13,902,766 microseconds
//    reset same box --    725,798 microseconds
//                       1,083,109 microseconds
//                         749,381 microseconds
//                         802,111 microseconds
//                         746,967 microseconds

void run_benchmark(int ITERS, bool RESET){
    auto start = high_resolution_clock::now();

    if (!RESET){
        // create a new sandbox every iteration
        for (int i = 0; i < ITERS; i++) {
            rlbox_sandbox_mylib sandbox;
            sandbox.create_sandbox();
            call_function(&sandbox);
            sandbox.destroy_sandbox();
            // if ((i % 1000) == 0) printf("done %d\n", i);
        }
    } else {
        rlbox_sandbox_mylib sandbox;

        // create one sandbox and reset it every iteration
        sandbox.create_sandbox();

        // std::__1::chrono::microseconds total_reset = duration_cast<microseconds>(start - start);
        for (int i = 0; i < ITERS; i++) {
            // auto iter_start = high_resolution_clock::now();
            call_function(&sandbox);
            // auto func_called = high_resolution_clock::now();
            sandbox.reset_sandbox();
            // auto sandbox_reset = high_resolution_clock::now();
            // total_reset += duration_cast<microseconds>(sandbox_reset - func_called);
            // if ((i % 1000) == 0) printf("done %d\n", i);
        }

        sandbox.destroy_sandbox();
    }

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
    if (RESET){
        printf("finished %d iters in %lld microseconds (w " RESETTING_OUT_COLOR "one shared sandbox" COLOR_RESET ")\n", ITERS, duration.count());
    } else {
        printf("finished %d iters in %lld microseconds (w " REMAKING_OUT_COLOR "a new sandbox every time" COLOR_RESET ")\n", ITERS, duration.count());
    }
    
}