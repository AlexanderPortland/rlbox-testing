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

#define release_assert(cond, msg) if (!(cond)) { fputs(msg, stderr); abort(); }

using namespace std;
using namespace rlbox;

// Define base type for mylib using the noop sandbox
RLBOX_DEFINE_BASE_TYPES_FOR(mylib, wasm2c);

// Declare callback function we're going to call from sandboxed code.
void hello_cb(rlbox_sandbox_mylib& _, tainted_mylib<const char*> str);

#define RESET true

int main(int argc, char const *argv[]) {
    rlbox_sandbox_mylib sandbox;
    sandbox.create_sandbox();

    // STR DEMO

    // define our strings
    const char* s1 = "hello this is ";
    size_t str1_sz = strlen(s1);
    const char *s2 = "a test string";
    size_t str2_sz = strlen(s2);

    // malloc them in the sandbox
    tainted_mylib<char*> taintedStr1 = sandbox.malloc_in_sandbox<char>(str1_sz);
    strncpy(taintedStr1.unverified_safe_pointer_because(str1_sz, "writing to region")
            , s1, str1_sz);
    tainted_mylib<char*> taintedStr2 = sandbox.malloc_in_sandbox<char>(str2_sz);
    strncpy(taintedStr2.unverified_safe_pointer_because(str2_sz, "writing to region")
            , s2, str2_sz);

    auto ok_str = sandbox.invoke_sandbox_function(naive_concat, taintedStr1, taintedStr2);

    char *str1 = taintedStr1.UNSAFE_unverified();
    char *str2 = taintedStr2.UNSAFE_unverified();
    char *str_final = ok_str.UNSAFE_unverified();

    printf("main: str1--%s\n", str1);
    printf("main: str2--%s\n", str2);
    printf("main: str_final--%s\n", str_final);

    #if RESET
    sandbox.reset_sandbox();
    #else
    sandbox.destroy_sandbox();
    sandbox.create_sandbox();
    #endif

    printf("main: str1--%s\n", str1);
    printf("main: str2--%s\n", str2);
    printf("main: str_final--%s\n", str_final);

    // NUM DEMO

    auto sec_num_res = sandbox.invoke_sandbox_function(leak_secret_num);
    auto const_sec_num_res = sandbox.invoke_sandbox_function(leak_const_secret_num);

    int *secret_num_ptr = sec_num_res.UNSAFE_unverified();
    int *const_secret_num_ptr = const_sec_num_res.UNSAFE_unverified();

    printf("main: secret_num--%d\n", *secret_num_ptr);
    printf("main: const_secret_num--%d\n", *const_secret_num_ptr);

//     sandbox.invoke_sandbox_function(hello);

    #if RESET
    sandbox.reset_sandbox();
    #else
    sandbox.destroy_sandbox();
    sandbox.create_sandbox();
    #endif

    printf("main: secret_num--%d\n", *secret_num_ptr);
    printf("main: const_secret_num--%d\n", *const_secret_num_ptr);

    // ADD DEMO

    auto C = 1234;
    auto D = 4321;
    auto ok_num2 = sandbox.invoke_sandbox_function(add, C, D)
                  .copy_and_verify([C, D](unsigned ret)
                                   {
        printf("main: we are adding %d and %d to get %d\n", C, D, ret);
        return ret == (C + D); });

    sandbox.destroy_sandbox();

    return 0;
}