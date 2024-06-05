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
#include "benchmark.hpp"
#include "demo.hpp"

#define release_assert(cond, msg) if (!(cond)) { fputs(msg, stderr); abort(); }

using namespace std;
using namespace std::chrono;
using namespace rlbox;

// Define base type for mylib using the noop sandbox
RLBOX_DEFINE_BASE_TYPES_FOR(mylib, wasm2c);

int main(int argc, char const *argv[]) {
    int ITERS = 1000000;
    bool RESET = true;

    // run_benchmark(1000000, RESET);

    rlbox_sandbox_mylib sandbox;
    sandbox.create_sandbox();

    add_demo(&sandbox, RESET);

    str_demo(&sandbox, RESET);

    secret_num_demo(&sandbox, RESET);

    sandbox.destroy_sandbox();

    return 0;
}