#ifndef DEMO_H
#define DEMO_H

// TODO: trim unneeded boilerplate here

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

// #define release_assert(cond, msg) if (!(cond)) { fputs(msg, stderr); abort(); }

using namespace std;
using namespace rlbox;

RLBOX_DEFINE_BASE_TYPES_FOR(mylib, wasm2c);

extern void add_demo(rlbox_sandbox_mylib*, bool);
extern void str_demo(rlbox_sandbox_mylib*, bool);
extern void secret_num_demo(rlbox_sandbox_mylib*, bool);

#endif