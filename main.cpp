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

int main(int argc, char const *argv[]) {
    rlbox_sandbox_mylib sandbox;
    sandbox.create_sandbox();

    sandbox.invoke_sandbox_function(hello);

    sandbox.destroy_sandbox();
    sandbox.create_sandbox();
    // printf("OK? = %d\n", ok_num);

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

    sandbox.destroy_sandbox();
    sandbox.create_sandbox();

    auto A = 50;
    auto B = 11101;
    auto ok_num = sandbox.invoke_sandbox_function(add, A, B)
                  .copy_and_verify([A, B](unsigned ret)
                                   {
        printf("main: we are adding %d and %d to get %d\n", A, B, ret);
        return ret == (A + B); });

    sandbox.destroy_sandbox();

    return 0;
}